/* FGA0003 - Compiladores 1
* Curso de Engenharia de Software
* Universidade de Brasilia (UnB)
* Analisador léxico incial desenvolvido com Flex
*/

%{
#include "scanner.tab.h"
#include <stdio.h>
#include <stdlib.h>
%}

%option noyywrap
%option yylineno

/* Definições de Expressões Regulares */
DIGIT           [0-9]
LETTER          [a-zA-Z_]
ALPHANUM        [a-zA-Z0-9_]

IDENTIFIER      {LETTER}{ALPHANUM}*
INT_LITERAL     {DIGIT}+
FLOAT_LITERAL   {DIGIT}+\.{DIGIT}+

/* Regras para Char e String */
CHAR_LITERAL    '([^'\\]|\\.)'
STRING_LITERAL  \"([^"\\]|\\.)*\"

WHITESPACE      [ \t\r\n]+
SINGLE_COMMENT  "//".*
MULTI_COMMENT   "/*"([^*]|\*+[^*/])*\*+"/"

%%

    /* Estrutura do Arquivo */
"public"    { return PUBLIC; }
"class"     { return CLASS; }
"static"    { return STATIC; }
"void"      { return VOID; }

    /* Tipos Primitivos */
"boolean"   { return BOOLEAN; }
"byte"      { return BYTE; }
"char"      { return CHAR; }
"double"    { return DOUBLE; }
"float"     { return FLOAT; }
"int"       { return INT; }
"long"      { return LONG; }
"short"     { return SHORT; }

    /* Controle de Fluxo */
"if"        { return IF; }
"else"      { return ELSE; }
"switch"    { return SWITCH; }
"case"      { return CASE; }
"default"   { return DEFAULT; }
"while"     { return WHILE; }
"do"        { return DO; }
"for"       { return FOR; }
"break"     { return BREAK; }
"continue"  { return CONTINUE; }
"return"    { return RETURN; }

    /* Modificadores e Literais Fixos */
"final"     { return FINAL; }
"true"      { return TOKEN_TRUE; }
"false"     { return TOKEN_FALSE; }
"null"      { return NULL_LITERAL; }

    /* Operadores */
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIV; }
"%"         { return MOD; }
"=="        { return EQ; }
"!="        { return NEQ; }
"<"         { return LT; }
"<="        { return LE; }
">"         { return GT; }
">="        { return GE; }
"&&"        { return AND; }
"||"        { return OR; }
"!"         { return NOT; }
"="         { return ASSIGN; }

    /* Delimitadores */
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }
";"         { return SEMI; }
","         { return COMMA; }

    /* Regras com Macros */
{FLOAT_LITERAL}   { return FLOAT_LITERAL; }
{INT_LITERAL} {
    yylval.intValue = atoi(yytext);
    return INT_LITERAL;
}
{CHAR_LITERAL}    { return CHAR_LITERAL; }
{STRING_LITERAL}  { return STRING_LITERAL; }
{IDENTIFIER}      { return IDENTIFIER; }

    /* Tratamento de erro para String não fechada */
\"([^"\\]|\\.)*   { printf("Erro lexico na linha %d: String nao encerrada\n", yylineno); }

    /* Descarte de Espaços e Comentários */
{WHITESPACE}      { /* ignora */ }
{SINGLE_COMMENT}  { /* ignora */ }
{MULTI_COMMENT}   { /* ignora */ }

.                 { printf("Erro lexico na linha %d: Caractere invalido '%s'\n", yylineno, yytext); }

%%

/* Função principal: chamará o parser ao ser executado */
int main(int argc, char **argv) {
    /* Se houver arquivo de entrada como parâmetro, redireciona o yyin */
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (f) {
            yyin = f;
        }
    }
    return yyparse();
}
