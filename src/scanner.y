%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE *yyin;
void yyerror(const char *s);
%}


%union {
    int intValue;
    float floatValue;
    char charValue;
    char* stringValue;

}

/* Identificadores e Valoração */
%token <stringValue> IDENTIFIER STRING_LITERAL
%token <charValue> CHAR_LITERAL
%token <floatValue> FLOAT_LITERAL
%token <intValue>  INT_LITERAL

/* Tokens do Java Procedural */
%token PUBLIC CLASS STATIC VOID 
%token BOOLEAN BYTE CHAR DOUBLE FLOAT INT LONG SHORT
%token IF ELSE SWITCH CASE DEFAULT WHILE DO FOR BREAK CONTINUE RETURN
%token FINAL TOKEN_TRUE TOKEN_FALSE NULL_LITERAL

/* Tokens de Operadores e Delimitadores */
%token PLUS MINUS MULT DIV MOD INC DEC 
%token EQ NEQ LT LE GT GE AND OR NOT ASSIGN
%token LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET SEMI COMMA

    /* Entrada e Saída */
%token PRINTLN PRINT

    /* Funções Matemáticas Nativas */
%token MATH_SQRT MATH_POW


/* A ordem define a precedência (de baixo para cima) */
%left OR
%left AND
%left EQ NEQ
%left LT LE GT GE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT


%%

programa:
    /* regra temporaria para o scanner */
    | programa comando {}
    ;

comando:
     tipo IDENTIFIER ASSIGN expressao SEMI {}
    | tipo IDENTIFIER SEMI {}
    | IF LPAREN expressao RPAREN LBRACE programa RBRACE {}
    | WHILE LPAREN expressao RPAREN LBRACE programa RBRACE {}
    | IDENTIFIER incrementacao SEMI {}
    | PRINTLN LPAREN expressao RPAREN SEMI {}
    | PRINT LPAREN expressao RPAREN SEMI {}
    ;

expressao:
      expressao PLUS expressao {}
    | expressao MINUS expressao {}
    | expressao MULT expressao {}
    | expressao DIV expressao {}
    | expressao MOD expressao {}
    
    | expressao AND expressao {}
    | expressao OR expressao {}
    | NOT expressao {}
    | expressao EQ expressao {}
    | expressao NEQ expressao {}
    | expressao LT expressao {}
    | expressao LE expressao {}
    | expressao GT expressao {}
    | expressao GE expressao {}
    
    | LPAREN expressao RPAREN {}
    
    | MATH_SQRT LPAREN expressao RPAREN {}
    | MATH_POW LPAREN expressao COMMA expressao RPAREN {}
    
    | IDENTIFIER {}
    | FLOAT_LITERAL {} 
    | INT_LITERAL {}
    | CHAR_LITERAL {}
    | STRING_LITERAL {}
    | TOKEN_TRUE {}
    | TOKEN_FALSE {}
    ;

tipo:
      INT {}
    | FLOAT {}
    | CHAR {}
    | BOOLEAN {}
    ;

incrementacao:
      INC {}
    | DEC {}
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintatico na linha %d: %s perto de '%s'\n", yylineno, s, yytext);
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

    if (yyparse() == 0) {
        printf("Analise concluida com sucesso! Nenhum erro sintatico encontrado.\n");
    }
    
    if (yyin) fclose(yyin);
    return 0;
}
