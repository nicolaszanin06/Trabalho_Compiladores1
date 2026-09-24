# **Analisador Léxico**

Esta página detalha a implementação da primeira fase do nosso compilador: a Análise Léxica. O objetivo desta etapa é ler o código-fonte em Java, agrupando os caracteres em unidades lógicas com significado, chamadas de *tokens*.

## **Ferramenta Utilizada**

Para o desenvolvimento do analisador léxico (também conhecido como *scanner*), a equipe utilizou a ferramenta **Flex** (Fast Lexical Analyzer Generator). O código-fonte desta etapa está centralizado no arquivo `scanner.l`.

## **Configurações Iniciais**

No arquivo `scanner.l`, foram definidas algumas opções importantes para o funcionamento do Flex:

* `%option noyywrap`: Indica que o analisador processará apenas um arquivo por vez, não precisando chamar a função `yywrap()` ao final da leitura.


* `%option yylineno`: Habilita o rastreamento automático do número das linhas, o que é essencial para apontar erros léxicos e sintáticos com precisão.


* Inclusão do cabeçalho `scanner.tab.h`: Necessária para que o Flex reconheça os códigos numéricos dos *tokens* gerados posteriormente pelo analisador sintático (Bison).



## **Definições de Expressões Regulares**

Para facilitar a leitura e a manutenção, foram criadas macros com expressões regulares básicas, que depois compõem regras mais complexas:

```lex
DIGIT           [0-9]
LETTER          [a-zA-Z_]
ALPHANUM        [a-zA-Z0-9_]

IDENTIFIER      {LETTER}{ALPHANUM}*
INT_LITERAL     {DIGIT}+
FLOAT_LITERAL   {DIGIT}+\.{DIGIT}+

```

### Strings e Caracteres

A captura de texto exigiu regras que considerassem caracteres de escape (como um `\n` ou uma aspa escapada `\"` dentro de uma string):

* **Caracteres:** `'([^'\\]|\\.)'`

* **Strings:** `\"([^"\\]|\\.)*\"`


## **Ignorando Espaços e Comentários**

O compilador não precisa processar espaços em branco ou comentários para entender a lógica do programa. As seguintes expressões foram criadas para consumir esses caracteres sem retornar nenhum *token* ao Bison (utilizando uma ação vazia `{ /* ignora */ }`):

* **Espaços em branco:** `[ \t\r\n]+`

* **Comentários de linha única:** `"//".*`

* **Comentários de bloco:** `"/*"([^*]|\*+[^*/])*\*+"/"`


Esta última regra de comentário de bloco utiliza uma lógica na própria regex para garantir que o comentário só termine quando encontrar a sequência exata `*/`.

## **Tratamento de Erros Léxicos**

O Flex processa o arquivo testando as regras de cima para baixo. Se nenhum padrão válido casar com o texto de entrada, o analisador aciona as regras de tratamento de erros no final do arquivo:

1. **Strings não encerradas:**
Se uma aspa dupla for aberta, mas o arquivo terminar ou não houver o fechamento adequado, a regra `\"([^"\\]|\\.)*` captura a anomalia e emite a mensagem:
`Erro lexico na linha X: String nao encerrada`.


2. **Caracteres Inválidos (Regra *Catch-all*):**
O caractere isolado `.` no final das regras age como um "pega-tudo". Se o símbolo lido não pertencer à linguagem (como `@` ou `#`), o programa o identifica e exibe:
`Erro lexico na linha X: Caractere invalido 'Y'`.


---

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| --- | --- | --- | --- |
| 1.0 | Criação da documentação do Analisador Léxico | 24/09/2026 | [Henrique](https://github.com/henryqma) |