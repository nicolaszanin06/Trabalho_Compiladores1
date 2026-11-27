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

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação do documento | 08/11/2026 | [Henrique](https://github.com/henryqma) |
| 2.0 | Atualização para a entrega final| 27/11/2026 | [Henrique](https://github.com/henryqma) |