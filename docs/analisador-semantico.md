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

Antes de emitir IR, a análise também verifica inicialização nos caminhos de
controle. Em uma junção, uma variável fica inicializada se tiver sido escrita
em todos os caminhos que chegam ao ponto. Em laços, a análise considera saídas
pela condição e por `break`. Cada caso de um `switch` pode ser alcançado
diretamente pelo seu rótulo. A [página da IR](codigo-intermediario.md) descreve
as conversões numéricas e as funções nativas.

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação do documento | 08/11/2026 | [Henrique](https://github.com/henryqma) |
