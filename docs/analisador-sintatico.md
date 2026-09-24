# **Analisador Sintático**

Esta página documenta a segunda fase do compilador: a Análise Sintática. Enquanto o analisador léxico identifica as "palavras" (tokens) do código, o analisador sintático (ou *parser*) verifica se essas palavras formam "frases" válidas de acordo com as regras gramaticais da linguagem Java.

## **Ferramenta Utilizada**

Para gerar o analisador sintático, a equipe utilizou a ferramenta **GNU Bison**. Toda a gramática livre de contexto e a configuração do *parser* estão definidas no arquivo `scanner.y`.

## **Integração com o Flex e Definição de Tokens**

O arquivo `scanner.y` atua como o mestre da definição de *tokens*. Todos os *tokens* identificados na etapa léxica são declarados inicialmente no Bison utilizando a diretiva `%token` (ex: `%token PUBLIC CLASS STATIC`, `%token INT_LITERAL`, etc.).

Durante a compilação, o Bison gera o arquivo de cabeçalho `scanner.tab.h`, que contém um `enum` com códigos numéricos únicos para cada *token*. O Flex inclui este cabeçalho e retorna estes códigos para o Bison toda vez que identifica um padrão válido.

## **Precedência de Operadores**

Para evitar ambiguidades na avaliação de expressões matemáticas, o Bison permite definir a associatividade e a precedência dos operadores. No nosso projeto, isso foi configurado utilizando a diretiva `%left`, que indica associatividade à esquerda, onde os operadores declarados por último têm maior precedência:

```yacc
%left PLUS MINUS
%left MULT DIV

```

Isso garante que multiplicações (`MULT`) e divisões (`DIV`) sejam processadas antes de adições (`PLUS`) e subtrações (`MINUS`).

## **Regras Gramaticais Iniciais**

Até o momento o *parser* conta com uma implementação parcial focada em validar declarações simples e uma estrutura condicional básica. As regras foram construídas da seguinte forma no arquivo `scanner.y`:

### 1. Estrutura do Programa

A regra raiz `program` é recursiva à esquerda, permitindo que o compilador leia múltiplos comandos sequenciais até o fim do arquivo:

```yacc
program:
    | program command {}
    ;

```

### 2. Comandos (`command`)

Atualmente, o *parser* reconhece duas estruturas principais como comandos válidos:

* **Declaração de Variáveis:** O formato `tipo IDENTIFIER SEMI` (ex: `int numero;`).


* **Estrutura Condicional:** O bloco `IF LPAREN condicional RPAREN LBRACE program RBRACE` (ex: `if (true) { ... }`).



### 3. Tipos e Condições Suportados

As regras auxiliares definem o que pode preencher os comandos estruturais:

* **`tipo`:** Suporta momentaneamente os tipos primitivos `INT`, `FLOAT` e `CHAR`.


* **`condicional`:** O subconjunto atual avalia literais booleanos puros, aceitando `TOKEN_TRUE` ou `TOKEN_FALSE` dentro dos parênteses do `if`.



## **Tratamento de Erros Sintáticos**

Caso o código-fonte fuja das regras estabelecidas na gramática, o Bison aciona automaticamente a função `yyerror`. Nós personalizamos esta função para que ela exiba uma mensagem amigável e precisa, utilizando as variáveis `yylineno` (linha do erro) e `yytext` (texto do *token* atual) integradas do Flex:

```c
void yyerror(const char *s) {
    fprintf(stderr, "Erro sintatico na linha %d: %s perto de '%s'\n", yylineno, s, yytext);
}

```

Isso permite ao desenvolvedor saber exatamente onde o compilador falhou ao tentar construir a Árvore Sintática.

---

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| --- | --- | --- | --- |
| 1.0 | Criação da documentação do Analisador Sintático | 24/09/2026 | [Henrique](https://github.com/henryqma) |