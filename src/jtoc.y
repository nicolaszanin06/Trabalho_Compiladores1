%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "tabela.h"

extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE *yyin;
extern int erros_lexicos;
int yywrap(void);
void yyerror(const char *s);
%}

%code requires {
#include "ast.h"
}

%union {
    char *stringValue;
    NoAST *no;
}

%token <stringValue> IDENTIFIER STRING_LITERAL
%token <stringValue> CHAR_LITERAL FLOAT_LITERAL INT_LITERAL

%type <no> expressao
%destructor { free($$); } <stringValue>

%token PUBLIC CLASS STATIC VOID
%token BOOLEAN BYTE CHAR DOUBLE FLOAT INT LONG SHORT STRING_TYPE
%token IF ELSE SWITCH CASE DEFAULT WHILE DO FOR BREAK CONTINUE RETURN
%token FINAL TOKEN_TRUE TOKEN_FALSE NULL_LITERAL
%token PLUS MINUS MULT DIV MOD INC DEC
%token EQ NEQ LT LE GT GE AND OR NOT ASSIGN
%token LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET SEMI COMMA
%token PRINTLN PRINT
%token MATH_SQRT MATH_POW

%left OR
%left AND
%left EQ NEQ
%left LT LE GT GE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT

%%

entrada:
      %empty
    | entrada linha
    ;

linha:
      expressao { imprimirAST($1); printf("\n"); }
    ;

expressao:
      expressao PLUS expressao  { $$ = criarNoOp('+', $1, $3); }
    | expressao MINUS expressao { $$ = criarNoOp('-', $1, $3); }
    | expressao MULT expressao  { $$ = criarNoOp('*', $1, $3); }
    | expressao DIV expressao   { $$ = criarNoOp('/', $1, $3); }
    | expressao MOD expressao   { $$ = criarNoOp('%', $1, $3); }
    | LPAREN expressao RPAREN   { $$ = $2; }
    | IDENTIFIER  { $$ = criarNoId($1); inserirSimbolo($1, "int"); free($1); }
    | INT_LITERAL { $$ = criarNoNum(atoi($1)); free($1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintatico na linha %d: %s perto de '%s'\n", yylineno, s, yytext);
}

int yywrap(void) {
    return 1;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            perror("Erro ao abrir o arquivo");
            return 1;
        }
        yyin = f;
    }

    printf("Árvore Sintática Abstrata (AST):\n");
    int resultado = yyparse();
    if (resultado == 0 && erros_lexicos == 0) {
        imprimirTabela();
    }

    if (yyin) fclose(yyin);
    return (resultado != 0 || erros_lexicos != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}