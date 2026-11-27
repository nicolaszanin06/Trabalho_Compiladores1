# **Analisador Semântico**

O analisador semântico está  integrado ao fluxo completo do compilador (`jtoc`), operando diretamente sobre a [AST e a Tabela de Símbolos](./ast-tabela.md) para garantir a validade semântica do programa antes das etapas de [geração de código intermediário](codigo-intermediario.md) e [geração de código C](geracao-c.md).

## **Escopo, resolução de nomes e tipos**

A análise percorre a AST construída pelo Bison, registra declarações na tabela de símbolos e associa cada uso à declaração visível no escopo atual ou em um escopo ancestral. Um mesmo identificador pode ser declarado em escopos distintos, enquanto redeclarações no mesmo escopo e usos de variáveis não declaradas resultam em erro semântico.

Além da resolução de escopos, o analisador realiza as seguintes validações:

* **Compatibilidade de tipos:** atribui tipos às expressões e verifica a compatibilidade entre tipos em inicializadores, atribuições e operandos.
* **Estruturas de controle:** exige expressões de condição do tipo booleano nas instruções `if`, `while` e `for`.
* **Contexto de desvio (`break`):** valida que o comando `break` seja utilizado exclusivamente dentro de um laço de repetição (`while`, `for`) ou de uma estrutura `switch`.
* **Estrutura `switch`:** exige seletor do tipo `int`, rótulos `case` com constantes inteiras distintas entre si e no máximo um bloco `default`.

## **Validação de literais inteiros**

Literais inteiros são validados em todos os modos de execução dentro da faixa de 32 bits com sinal (`-2147483648` a `2147483647`). A conversão decimal verifica erros de formatação e estouros (*overflow*) antes de converter o valor para `int32_t`.

A mesma rotina de validação atende tanto às expressões gerais quanto às constantes dos rótulos `case`. Além disso, a AST marca explicitamente o valor mínimo inteiro (`-2147483648`) para que as etapas posteriores de geração de código possam emiti-lo corretamente como uma constante única.

## **Análise de inicialização e fluxo de controle**

Antes da geração de código intermediário (`--ir`) ou código C (`--c` ou `-o`), o analisador executa uma verificação de inicialização definitiva nos caminhos de controle do programa:

* **Junções de fluxo:** em pontos de convergência (como após um `if`/`else`), uma variável só é considerada inicializada se tiver sido escrita em todos os caminhos possíveis que alcançam aquele ponto.
* **Laços e `switch`:** nos laços, a análise considera tanto a saída pela condição quanto desvios via `break`; no `switch`, considera que cada `case` pode ser alcançado diretamente pelo seu respectivo rótulo.
* **Avaliação de condições constantes:** condições constantes abrangem literais booleanos, operadores `!`, `&&`, `||`, igualdade booleana e comparações entre expressões constantes inteiras (por exemplo, `int x; if (1 == 1) { x = 2; } System.out.print(x);` é aceito). A avaliação inteira é compartilhada com os rótulos `case` do `switch`, opera em 32 bits com retorno circular no *overflow* e preserva a avaliação em curto-circuito. Expressões com divisão por zero, variáveis ou valores de ponto flutuante permanecem indeterminadas nessa análise.

Essa verificação adicional de inicialização ocorre quando o compilador é executado com `--ir`, `--c` ou `-o` (independentemente de `--debug`), rejeitando leituras de variáveis não inicializadas antes da emissão de IR ou C. Já no modo padrão de validação do front-end, construções como `int x; System.out.print(x);` continuam sendo aceitas pela checagem básica de escopo e tipos. Consulte a documentação de [Código intermediário](codigo-intermediario.md) para detalhes sobre conversões numéricas e funções nativas.

## Tipagem do subconjunto

| Contexto | Regra implementada |
| --- | --- |
| Inicialização e `=` | Mesmo tipo ou promoção de `int` para `float`; não há conversão inversa implícita. |
| `+`, `-`, `*`, `/`, `%` e menos unário | Apenas `int`/`float`; resultado `float` se algum operando for `float`, caso contrário `int`. |
| `+=`, `-=`, `*=`, `/=`, `++`, `--` | Destino numérico; compostas seguem a compatibilidade de atribuição. `int += float` é rejeitado, sem o cast implícito do Java completo. |
| `<`, `<=`, `>`, `>=` | Apenas números; resultado `boolean`. |
| `==`, `!=` | Tipos iguais ou par numérico misto; resultado `boolean`. Em `String`, igualdade significa identidade de referência. |
| `!`, `&&`, `\|\|` | Apenas booleanos; resultado `boolean`. |
| `Math.sqrt`, `Math.pow` | Um e dois argumentos numéricos, respectivamente; resultado `float`. |

`char` não participa de promoção numérica e `String` não admite concatenação. A inserção de uma declaração precede a análise de seu inicializador; `int x = x;` resolve o nome, mas é rejeitado pela checagem de inicialização nos modos de geração.

## Faixa de literais float

Em todos os modos, `validar_literal_float` usa `strtof` e exige consumo do lexema completo. Rejeita overflow para infinito/estouro e um literal decimal matematicamente não nulo convertido para `0.0f` por underflow. Zeros com muitas casas são válidos; subnormais representáveis não são rejeitados apenas por `ERANGE`.

Em um alvo com `float` IEEE 754 de 32 bits, o maior valor finito é aproximadamente `3,402823466 × 10^38` e o menor subnormal positivo é aproximadamente `1,401298464 × 10^-45`. A aceitação considera o arredondamento de `strtof`: um decimal próximo do menor subnormal pode ser aceito se arredondar para um valor não nulo. A escrita científica serve aqui apenas para explicar os limites; o scanner não aceita expoentes.

A faixa é a dos **literais**, não uma proibição geral de resultados não finitos durante a execução. Operações matemáticas e ponto flutuante gerado seguem o contrato do [backend C](geracao-c.md).

## Precisão e limites da análise de caminhos

O estado de fluxo combina um vetor de inicialização indexado por `id_simbolo` e uma marca de alcançabilidade. A junção faz interseção apenas entre caminhos que chegam ao ponto. `break` registra uma saída da estrutura mais próxima e encerra o caminho corrente. Atribuição simples verifica o lado direito antes de marcar o destino; compostas e incremento/decremento também leem o destino.

Um laço com condição desconhecida pode executar zero vezes. Um laço comprovadamente verdadeiro só deixa saídas alcançáveis por `break`; o `for` considera inicialização e atualização na ordem de execução. Em `switch`, cada rótulo pode receber entrada direta, além de fall-through; sem `default`, existe ainda o caminho sem caso correspondente. O seletor constante não elimina essas alternativas na análise atual.

Não há propagação de valores de variáveis: `boolean b = true; int x; if (b) { x = 1; } System.out.print(x);` é rejeitado na geração, embora trocar `b` por `true` torne o exemplo aceito. Leituras em operandos comprovadamente dispensados pelo curto-circuito e trechos inalcançáveis são ignoradas **na checagem de inicialização**. A análise básica de nomes e tipos ainda visita a árvore toda, inclusive esses trechos. Avaliar constantes para validar fluxo e `case` não é um passe de otimização.

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação do documento | 08/11/2026 | [Henrique](https://github.com/henryqma) |
| 2.0 | Atualização para a entrega final| 27/11/2026 | [Henrique](https://github.com/henryqma) |
