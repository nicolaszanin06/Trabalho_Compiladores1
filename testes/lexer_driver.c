/* Executavel de teste: observa yylex sem passar pela gramatica do parser. */
#include <stdio.h>
#include "scanner.tab.h"

extern int yylex(void);
extern int yylineno;
extern char *yytext;
YYSTYPE yylval;

#define TOKEN(nome) case nome: return #nome;
static const char *nome_token(int token) {
    switch (token) {
        TOKEN(PUBLIC) TOKEN(CLASS) TOKEN(STATIC) TOKEN(VOID)
        TOKEN(BOOLEAN) TOKEN(BYTE) TOKEN(CHAR) TOKEN(DOUBLE)
        TOKEN(FLOAT) TOKEN(INT) TOKEN(LONG) TOKEN(SHORT)
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
        if (token == INT_LITERAL) printf("|%d", yylval.intValue);
        putchar('\n');
    }
    return 0;
}
