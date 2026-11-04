%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "tabela.h"

extern int yylex();
extern int yylineno;
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

%type <no> expr stmt
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

%right ASSIGN
%left PLUS MINUS

%%

input:
      %empty
    | input stmt
    ;

stmt:
      INT IDENTIFIER SEMI {
          if (buscarSimbolo($2)) yyerror("Redeclaracao de variavel");
          else inserirSimbolo($2, TIPO_INT);
          free($2);
          $$ = NULL;
      }
    | IDENTIFIER ASSIGN expr {
          Simbolo *s = buscarSimbolo($1);
          if (!s) {
              yyerror("Identificador nao declarado");
              $$ = criarNoNum(0);
          } else {
              $$ = criarNoOp('=', criarNoId($1, s->tipo), $3);
          }
          free($1);
          printf("\nAST: ");
          imprimirAST($$);
      }
    | expr {
          $$ = $1;
          printf("\nAST: ");
          imprimirAST($$);
      }
    ;

expr:
      expr PLUS expr {
          if (!tiposCompativeis($1->tipo, $3->tipo)) yyerror("Tipos incompativeis para '+'");
          $$ = criarNoOp('+', $1, $3);
      }
    | expr MINUS expr {
          if (!tiposCompativeis($1->tipo, $3->tipo)) yyerror("Tipos incompativeis para '-'");
          $$ = criarNoOp('-', $1, $3);
      }
    | INT_LITERAL { $$ = criarNoNum(atoi($1)); free($1); }
    | IDENTIFIER {
          Simbolo *s = buscarSimbolo($1);
          if (!s) {
              yyerror("Identificador nao declarado");
              $$ = criarNoNum(0);
          } else {
              $$ = criarNoId($1, s->tipo);
          }
          free($1);
      }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro semantico: %s\n", s);
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

    yyparse();
    printf("\nTabela de Simbolos:\n");
    imprimirTabela();

    if (yyin) fclose(yyin);
    return 0;
}