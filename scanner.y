%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yylineno;
extern char* yytext;
void yyerror(const char *s);
%}

/* Tokens do Java Procedural */
%token PUBLIC CLASS STATIC VOID
%token BOOLEAN BYTE CHAR DOUBLE FLOAT INT LONG SHORT
%token IF ELSE SWITCH CASE DEFAULT WHILE DO FOR BREAK CONTINUE RETURN
%token FINAL BOOL_LITERAL NULL_LITERAL

/* Tokens de Operadores e Delimitadores */
%token PLUS MINUS MULT DIV MOD
%token EQ NEQ LT LE GT GE AND OR NOT ASSIGN
%token LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET SEMI COMMA

/* Identificadores e Valoração */
%token INT_LITERAL FLOAT_LITERAL IDENTIFIER
%token STRING_LITERAL CHAR_LITERAL

%%

program:
    /* regra temporaria para o scanner */
    | program token
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
    | BOOL_LITERAL { printf("BOOL_LITERAL "); }
    | NULL_LITERAL { printf("NULL_LITERAL "); }
    | PLUS { printf("PLUS "); }
    | MINUS { printf("MINUS "); }
    | MULT { printf("MULT "); }
    | DIV { printf("DIV "); }
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
    | SEMI { printf("SEMI\n"); }
    | COMMA { printf("COMMA "); }
    | FLOAT_LITERAL { printf("FLOAT_LITERAL(%s) ", yytext); }
    | INT_LITERAL { printf("NUM(%s) ", yytext); }
    | CHAR_LITERAL   { printf("CHAR_LITERAL(%s) ", yytext); }
    | STRING_LITERAL { printf("STRING_LITERAL(%s) ", yytext); }
    | IDENTIFIER { printf("ID(%s) ", yytext); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintatico na linha %d: %s perto de '%s'\n", yylineno, s, yytext);
}

int main(int argc, char **argv) {
    yyparse();
    return 0;
}
