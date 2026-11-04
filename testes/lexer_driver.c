/* Executavel de teste: observa yylex sem passar pela gramatica do parser. */
#include <stdio.h>
#include <stdlib.h>
#include "jtoc.tab.h"

extern int yylex(void);
extern int yylineno;
extern char *yytext;
YYSTYPE yylval;
int yywrap(void) { return 1; }

#define TOKEN(nome) case nome: return #nome;
static const char *nome_token(int token) {
    switch (token) {
        TOKEN(PUBLIC) TOKEN(CLASS) TOKEN(STATIC) TOKEN(VOID)
        TOKEN(BOOLEAN) TOKEN(BYTE) TOKEN(CHAR) TOKEN(DOUBLE)
        TOKEN(FLOAT) TOKEN(INT) TOKEN(LONG) TOKEN(SHORT) TOKEN(STRING_TYPE)
        TOKEN(PRINTLN) TOKEN(PRINT) TOKEN(MATH_SQRT) TOKEN(MATH_POW)
        TOKEN(IF) TOKEN(ELSE) TOKEN(SWITCH) TOKEN(CASE) TOKEN(DEFAULT)
        TOKEN(WHILE) TOKEN(DO) TOKEN(FOR) TOKEN(BREAK) TOKEN(CONTINUE)
        TOKEN(RETURN) TOKEN(FINAL) TOKEN(TOKEN_TRUE) TOKEN(TOKEN_FALSE)
        TOKEN(NULL_LITERAL) TOKEN(PLUS) TOKEN(MINUS) TOKEN(MULT)
        TOKEN(DIV) TOKEN(MOD) TOKEN(INC) TOKEN(DEC) TOKEN(EQ) TOKEN(NEQ)
        TOKEN(LT) TOKEN(LE) TOKEN(GT) TOKEN(GE) TOKEN(AND) TOKEN(OR)
        TOKEN(NOT) TOKEN(ASSIGN) TOKEN(LBRACE) TOKEN(RBRACE)
        TOKEN(LPAREN) TOKEN(RPAREN) TOKEN(LBRACKET) TOKEN(RBRACKET)
        TOKEN(SEMI) TOKEN(COMMA) TOKEN(INT_LITERAL) TOKEN(FLOAT_LITERAL)
        TOKEN(IDENTIFIER) TOKEN(STRING_LITERAL) TOKEN(CHAR_LITERAL)
        default: return "TOKEN_DESCONHECIDO";
    }
}

int main(void) {
    int token;
    while ((token = yylex()) != 0) {
        printf("%s|%s|%d", nome_token(token), yytext, yylineno);
        if (token == INT_LITERAL) printf("|%s", yylval.stringValue);
        putchar('\n');
        if (token == INT_LITERAL || token == FLOAT_LITERAL ||
            token == CHAR_LITERAL || token == STRING_LITERAL || token == IDENTIFIER)
            free(yylval.stringValue);
    }
    return 0;
}
