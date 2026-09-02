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
%type <intValue> expr


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
    | program statement
    ;

statement:
      token
    | expr SEMI { printf("RESULTADO(%d)\n", $1); }
    ;

token:
    PUBLIC { printf("PUBLIC "); }
    | CLASS { printf("CLASS "); }
    | STATIC { printf("STATIC "); }
    | VOID { printf("VOID "); }
    | BOOLEAN { printf("BOOLEAN "); }
    | BYTE { printf("BYTE "); }
    | CHAR { printf("CHAR "); }
    | DOUBLE { printf("DOUBLE "); }
    | FLOAT { printf("FLOAT "); }
    | INT { printf("INT "); }
    | LONG { printf("LONG "); }
    | SHORT { printf("SHORT "); }
    | IF { printf("IF "); }
    | ELSE { printf("ELSE "); }
    | SWITCH { printf ("SWITCH "); }
    | CASE { printf("CASE "); }
    | DEFAULT { printf("DEFAULT "); }
    | WHILE { printf("WHILE "); }
    | DO { printf("DO "); }
    | FOR { printf("FOR "); }
    | BREAK { printf("BREAK "); }
    | CONTINUE { printf("CONTINUE "); }
    | RETURN { printf("RETURN "); }
    | FINAL { printf("FINAL "); }
    | TOKEN_TRUE   { printf("TOKEN_TRUE "); }
    | TOKEN_FALSE  { printf("TOKEN_FALSE "); }
    | NULL_LITERAL { printf("NULL_LITERAL "); }
    | MOD { printf("MOD "); }
    | EQ { printf("EQ "); }
    | NEQ { printf("NEQ "); }
    | LT { printf("LT "); }
    | LE { printf("LE "); }
    | GT { printf("GT "); }
    | GE { printf("GE "); }
    | AND { printf("AND "); }
    | OR { printf("OR "); }
    | NOT { printf("NOT "); }
    | ASSIGN { printf("ASSIGN "); }
    | LBRACE { printf("LBRACE\n"); }
    | RBRACE { printf("RBRACE\n"); }
    | LPAREN { printf("LPAREN "); }
    | RPAREN { printf("RPAREN "); }
    | LBRACKET { printf("LBRACKET "); }
    | RBRACKET { printf("RBRACKET "); }
    | COMMA { printf("COMMA "); }
    | FLOAT_LITERAL { printf("FLOAT_LITERAL(%s) ", yytext); }
    | CHAR_LITERAL   { printf("CHAR_LITERAL(%s) ", yytext); }
    | STRING_LITERAL { printf("STRING_LITERAL(%s) ", yytext); }
    | IDENTIFIER { printf("ID(%s) ", yytext); }
    ;



    expr:
          expr PLUS expr     { $$ = $1 + $3; }
        | expr MINUS expr    { $$ = $1 - $3; }
        | expr MULT expr     { $$ = $1 * $3; }
        | expr DIV expr      { $$ = $1 / $3; }
        | LPAREN expr RPAREN { $$ = $2; }
        | INT_LITERAL        { $$ = $1; }
        ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintatico na linha %d: %s perto de '%s'\n", yylineno, s, yytext);
}

