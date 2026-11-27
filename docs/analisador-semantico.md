# Analisador Semântico

Atualmente, o analisador semântico encontra-se em fase de desenvolvimento, com a infraestrutura base ([AST e Tabela de Símbolos](./ast-tabela.md)) já concluída e integrada.


## Resolução de nomes e tipos

A análise percorre a AST construída pelo Bison, registra declarações na tabela
de símbolos e associa cada uso à declaração visível no escopo atual ou em um
ancestral. Um nome pode aparecer em escopos diferentes. Redeclarações no mesmo
escopo e usos sem declaração geram erros.

A análise atribui tipos às expressões, verifica a compatibilidade de
inicializadores, atribuições e operandos, e exige condições booleanas no
`if`, `while` e `for`. O `break` exige um laço ou `switch` envolvente.
Um `switch` exige seletor `int`, casos constantes inteiros distintos e até um
`default`.

Literais inteiros são validados em todos os modos na faixa de 32 bits com
sinal: `-2147483648` a `2147483647`. A conversão decimal verifica erros e
estouros antes de converter para `int32_t`. A mesma função atende expressões
gerais e constantes de `case`. A AST marca o mínimo inteiro para que a IR o
emita como uma constante única.

Antes de emitir IR, a análise também verifica inicialização nos caminhos de
controle. Em uma junção, uma variável fica inicializada se tiver sido escrita
em todos os caminhos que chegam ao ponto. Em laços, a análise considera saídas
pela condição e por `break`. Cada caso de um `switch` pode ser alcançado
diretamente pelo seu rótulo. A [página da IR](codigo-intermediario.md) descreve
as conversões numéricas e as funções nativas.

Essa verificação adicional ocorre somente com `--ir`, independentemente de
`--debug`. A execução padrão ainda aceita, por exemplo, `int x; System.out.print(x);`;
com `--ir`, a leitura sem inicialização é rejeitada e stdout fica vazio.

Condições constantes incluem literais booleanos, `!`, `&&`, `||`, igualdade
booleana e comparações de expressões constantes inteiras. Por exemplo,
`int x; if (1 == 1) { x = 2; } System.out.print(x);` é aceito. A avaliação
inteira é compartilhada com os casos de `switch`, usa 32 bits com retorno
circular no overflow e preserva o curto-circuito. Divisão por zero, variáveis
e condições de ponto flutuante permanecem desconhecidas nessa análise.

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação do documento | 08/11/2026 | [Henrique](https://github.com/henryqma) |
