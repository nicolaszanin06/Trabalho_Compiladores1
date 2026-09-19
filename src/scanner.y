%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yylineno;
extern char* yytext;
void yyerror(const char *s);
%}


%union {
    int intValue;
}

%token <intValue>  INT_LITERAL


/* Tokens do Java Procedural */
%token PUBLIC CLASS STATIC VOID
%token BOOLEAN BYTE CHAR DOUBLE FLOAT INT LONG SHORT
%token IF ELSE SWITCH CASE DEFAULT WHILE DO FOR BREAK CONTINUE RETURN
%token FINAL TOKEN_TRUE TOKEN_FALSE NULL_LITERAL

/* Tokens de Operadores e Delimitadores */
%token PLUS MINUS MULT DIV MOD
%token EQ NEQ LT LE GT GE AND OR NOT ASSIGN
%token LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET SEMI COMMA


%left PLUS MINUS
%left MULT DIV

/* Identificadores e Valoração */
%token FLOAT_LITERAL IDENTIFIER
%token STRING_LITERAL CHAR_LITERAL

%%

program:
    /* regra temporaria para o scanner */
    | program command
    ;

command:
        tipo IDENTIFIER SEMI {}
    |   IF LPAREN condicional RPAREN LBRACE program RBRACE {}
    ;

condicional:
        TOKEN_TRUE {}
    |   TOKEN_FALSE {}
    ; 

tipo:
        INT {}
    |   FLOAT {}
    |   CHAR {}
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintatico na linha %d: %s perto de '%s'\n", yylineno, s, yytext);
}

