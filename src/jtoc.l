/* FGA0003 - Compiladores 1
* Curso de Engenharia de Software
* Universidade de Brasilia (UnB)
* Analisador léxico incial desenvolvido com Flex
*/

%{
#include "jtoc.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int erros_lexicos = 0;
FILE *saida_lexica;
int linha_comentario = 1;

static char *copiar_lexema(const char *texto) {
    char *copia = strdup(texto);
    if (copia == NULL) {
        fprintf(stderr, "Memoria insuficiente para o analisador lexico.\n");
        exit(EXIT_FAILURE);
    }
    return copia;
}
%}

%option yylineno
%option nounput    
%option noinput
%x COMENTARIO

/* Definições de Expressões Regulares */
DIGIT           [0-9]
LETTER          [a-zA-Z_]
ALPHANUM        [a-zA-Z0-9_]

IDENTIFIER      {LETTER}{ALPHANUM}*
INT_LITERAL     {DIGIT}+
FLOAT_LITERAL   {DIGIT}+\.{DIGIT}+

/* Regras para Char e String */
ESCAPE          \\[btnfr\"'\\]
CHAR_LITERAL    '([^'\\\r\n]|{ESCAPE})'
STRING_LITERAL  \"([^"\\\r\n]|{ESCAPE})*\"

WHITESPACE      [ \t\r\n]+
SINGLE_COMMENT  "//".*

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
"String"    { return STRING_TYPE; }

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

    /* Entrada e Saída */

"System.out.println"    { return PRINTLN; }
"System.out.print"      { return PRINT; }

    /* Funções Matemáticas Nativas */
"Math.sqrt"     { return MATH_SQRT; }
"Math.pow"      { return MATH_POW; }

    /* Modificadores e Literais Fixos */
"final"     { return FINAL; }
"true"      { return TOKEN_TRUE; }
"false"     { return TOKEN_FALSE; }
"null"      { return NULL_LITERAL; }

    /* Operadores */
"++"        { return INC; }
"--"        { return DEC; }
"+="        { return ADD_ASSIGN; }
"-="        { return SUB_ASSIGN; }
"*="        { return MUL_ASSIGN; }
"/="        { return DIV_ASSIGN; }
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
":"         { return COLON; }

    /* Regras com Macros */
{FLOAT_LITERAL}   { yylval.stringValue = copiar_lexema(yytext); return FLOAT_LITERAL; }
{INT_LITERAL}     { yylval.stringValue = copiar_lexema(yytext); return INT_LITERAL; }
{CHAR_LITERAL}    { yylval.stringValue = copiar_lexema(yytext); return CHAR_LITERAL; }
{STRING_LITERAL}  { yylval.stringValue = copiar_lexema(yytext); return STRING_LITERAL; }
{IDENTIFIER}      { yylval.stringValue = copiar_lexema(yytext); return IDENTIFIER; }

    /* Literais fechados que nao passaram pelas regras validas acima. */
\"([^"\\\r\n]|\\[^\r\n])*\" { erros_lexicos++; fprintf(saida_lexica ? saida_lexica : stdout, "Erro lexico na linha %d: Escape invalido em String\n", yylineno); }
'([^'\\\r\n]|\\[^\r\n])' { erros_lexicos++; fprintf(saida_lexica ? saida_lexica : stdout, "Erro lexico na linha %d: Escape invalido em char\n", yylineno); }

    /* Tratamento de erro para String não fechada */
\"([^"\\\r\n]|\\[^\r\n])*\\?   { erros_lexicos++; fprintf(saida_lexica ? saida_lexica : stdout, "Erro lexico na linha %d: String nao encerrada\n", yylineno); }

    /* Descarte de Espaços e Comentários */
{WHITESPACE}      { /* ignora */ }
{SINGLE_COMMENT}  { /* ignora */ }
"/*"             { linha_comentario = yylineno; BEGIN(COMENTARIO); }
<COMENTARIO>"*/"  { BEGIN(INITIAL); }
<COMENTARIO>\n    { /* ignora; yylineno conta a quebra de linha */ }
<COMENTARIO>.     { /* ignora */ }
<COMENTARIO><<EOF>> { erros_lexicos++; fprintf(saida_lexica ? saida_lexica : stdout, "Erro lexico na linha %d: Comentario nao encerrado\n", linha_comentario); BEGIN(INITIAL); return 0; }

.                 { erros_lexicos++; fprintf(saida_lexica ? saida_lexica : stdout, "Erro lexico na linha %d: Caractere invalido '%s'\n", yylineno, yytext); }

%%
