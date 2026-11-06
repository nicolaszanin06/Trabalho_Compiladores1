%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_simbolos.h"

extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE *yyin;
extern int erros_lexicos;
int yywrap(void);
void yyerror(const char *s);
%}

/* Compartilha o tipo da AST com o parser e o scanner. */
%code requires {
#include "ast.h"
}

/* Raiz usada pelas acoes da gramatica e impressao opcional de debug. */
%code {
void debug_compiler(void);

NoAST *raiz = NULL;
}

%union {
    char* stringValue;
    NoAST *no;
}

/* Identificadores e Valoração */
%token <stringValue> IDENTIFIER STRING_LITERAL
%token <stringValue> CHAR_LITERAL FLOAT_LITERAL INT_LITERAL

%type <no> programa comando expressao tipo incrementacao
%type <no> bloco declaracao atribuicao atualizacao operador_atribuicao
%type <no> inicializacao_for condicao_for atualizacao_for casos caso
/* Libera tokens e subarvores descartados quando ocorre erro sintatico. */
%destructor { free($$); } <stringValue>
%destructor { liberar_ast($$); } <no>
%start inicio

/* Tokens do Java Procedural */
%token PUBLIC CLASS STATIC VOID 
%token BOOLEAN BYTE CHAR DOUBLE FLOAT INT LONG SHORT STRING_TYPE
%token IF ELSE SWITCH CASE DEFAULT WHILE DO FOR BREAK CONTINUE RETURN
%token FINAL TOKEN_TRUE TOKEN_FALSE NULL_LITERAL

/* Tokens de Operadores e Delimitadores */
%token PLUS MINUS MULT DIV MOD INC DEC 
%token EQ NEQ LT LE GT GE AND OR NOT ASSIGN
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN
%token LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET SEMI COMMA COLON

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

/* A raiz so e guardada quando toda a entrada foi reconhecida. */
inicio:
      programa { raiz = $1; }
    ;

programa:
      %empty { $$ = criar_no("Bloco"); }
    | programa comando { adicionar_filho($1, $2); $$ = $1; }
    ;

comando:
      declaracao SEMI { $$ = $1; }
    | atualizacao SEMI { $$ = $1; }
    | bloco { $$ = $1; }
    | IF LPAREN expressao RPAREN bloco ELSE bloco { $$ = montar_no("If", $3, $5, $7); }
    | IF LPAREN expressao RPAREN bloco { $$ = montar_no("If", $3, $5, NULL); }
    | WHILE LPAREN expressao RPAREN bloco { $$ = montar_no("While", $3, $5, NULL); }
    | FOR LPAREN inicializacao_for SEMI condicao_for SEMI atualizacao_for RPAREN bloco {
          $$ = montar_no("For", $3, $5, $7);
          adicionar_filho($$, $9);
      }
    | SWITCH LPAREN expressao RPAREN LBRACE casos RBRACE { $$ = montar_no("Switch", $3, $6, NULL); }
    | BREAK SEMI { $$ = criar_no("Break"); }
    | PRINTLN LPAREN expressao RPAREN SEMI { $$ = montar_no("Println", $3, NULL, NULL); }
    | PRINT LPAREN expressao RPAREN SEMI { $$ = montar_no("Print", $3, NULL, NULL); }
    ;

bloco:
      LBRACE programa RBRACE { $$ = $2; }
    ;

declaracao:
      tipo IDENTIFIER ASSIGN expressao { $$ = montar_no("Declaracao", $1, criar_identificador($2), $4); }
    | tipo IDENTIFIER { $$ = montar_no("Declaracao", $1, criar_identificador($2), NULL); }
    ;

atribuicao:
      IDENTIFIER operador_atribuicao expressao {
          adicionar_filho($2, criar_identificador($1));
          adicionar_filho($2, $3);
          $$ = $2;
      }
    ;

operador_atribuicao:
      ASSIGN { $$ = criar_no("="); }
    | ADD_ASSIGN { $$ = criar_no("+="); }
    | SUB_ASSIGN { $$ = criar_no("-="); }
    | MUL_ASSIGN { $$ = criar_no("*="); }
    | DIV_ASSIGN { $$ = criar_no("/="); }
    ;

atualizacao:
      atribuicao { $$ = $1; }
    | IDENTIFIER incrementacao { adicionar_filho($2, criar_identificador($1)); $$ = $2; }
    ;

/* Vazio mantem as quatro posicoes dos filhos de For, mesmo em for (;;). */
inicializacao_for:
      %empty { $$ = criar_no("Vazio"); }
    | declaracao { $$ = $1; }
    | atribuicao { $$ = $1; }
    ;

condicao_for:
      %empty { $$ = criar_no("Vazio"); }
    | expressao { $$ = $1; }
    ;

atualizacao_for:
      %empty { $$ = criar_no("Vazio"); }
    | atualizacao { $$ = $1; }
    ;

casos:
      %empty { $$ = criar_no("Casos"); }
    | casos caso { adicionar_filho($1, $2); $$ = $1; }
    ;

caso:
      CASE expressao COLON programa { $$ = montar_no("Case", $2, $4, NULL); }
    | DEFAULT COLON programa { $$ = montar_no("Default", $3, NULL, NULL); }
    ;

expressao:
      expressao PLUS expressao { $$ = montar_no("+", $1, $3, NULL); }
    | expressao MINUS expressao { $$ = montar_no("-", $1, $3, NULL); }
    | expressao MULT expressao { $$ = montar_no("*", $1, $3, NULL); }
    | expressao DIV expressao { $$ = montar_no("/", $1, $3, NULL); }
    | expressao MOD expressao { $$ = montar_no("%", $1, $3, NULL); }
    
    | expressao AND expressao { $$ = montar_no("&&", $1, $3, NULL); }
    | expressao OR expressao { $$ = montar_no("||", $1, $3, NULL); }
    | MINUS expressao %prec NOT { $$ = montar_no("-", $2, NULL, NULL); }
    | NOT expressao { $$ = montar_no("!", $2, NULL, NULL); }
    | expressao EQ expressao { $$ = montar_no("==", $1, $3, NULL); }
    | expressao NEQ expressao { $$ = montar_no("!=", $1, $3, NULL); }
    | expressao LT expressao { $$ = montar_no("<", $1, $3, NULL); }
    | expressao LE expressao { $$ = montar_no("<=", $1, $3, NULL); }
    | expressao GT expressao { $$ = montar_no(">", $1, $3, NULL); }
    | expressao GE expressao { $$ = montar_no(">=", $1, $3, NULL); }
    
    | LPAREN expressao RPAREN { $$ = $2; }
    
    | MATH_SQRT LPAREN expressao RPAREN { $$ = montar_no("Math.sqrt", $3, NULL, NULL); }
    | MATH_POW LPAREN expressao COMMA expressao RPAREN { $$ = montar_no("Math.pow", $3, $5, NULL); }
    
    | IDENTIFIER { $$ = criar_identificador($1); }
    | FLOAT_LITERAL { $$ = criar_folha($1); }
    | INT_LITERAL { $$ = criar_folha($1); }
    | CHAR_LITERAL { $$ = criar_folha($1); }
    | STRING_LITERAL { $$ = criar_folha($1); }
    | TOKEN_TRUE { $$ = criar_no("true"); }
    | TOKEN_FALSE { $$ = criar_no("false"); }
    ;

tipo:
      INT { $$ = criar_no("int"); }
    | FLOAT { $$ = criar_no("float"); }
    | CHAR { $$ = criar_no("char"); }
    | BOOLEAN { $$ = criar_no("boolean"); }
    | STRING_TYPE { $$ = criar_no("String"); }
    ;

incrementacao:
      INC { $$ = criar_no("++"); }
    | DEC { $$ = criar_no("--"); }
    ;

%%

/* Toda a saida de inspecao fica aqui; a analise nao depende desta funcao. */
void debug_compiler(void) {
    printf("AST:\n");
    imprimir_ast(raiz, 0);
    imprimir_tabela();
}

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintatico na linha %d: %s perto de '%s'\n", yylineno, s, yytext);
}

int yywrap(void) {
    return 1;
}

int main(int argc, char **argv) {
    int imprimir_debug = 0;
    const char *arquivo = NULL;
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--debug") == 0) {
            imprimir_debug = 1;
        } else if (argv[i][0] == '-' || arquivo != NULL) {
            fprintf(stderr, "Uso: %s [--debug] [arquivo.java]\n", argv[0]);
            return EXIT_FAILURE;
        } else {
            arquivo = argv[i];
        }
    }
    if (arquivo != NULL) {
        FILE *f = fopen(arquivo, "r");
        if (!f) {
            perror("Erro ao abrir o arquivo");
            return 1;
        }
        yyin = f;
    }

    int resultado = yyparse();
    if (resultado == 0 && erros_lexicos == 0) {
        montar_tabela(raiz, NULL);
        if (erros_semanticos == 0) {
            printf("Analise concluida com sucesso! Nenhum erro sintatico encontrado.\n");
            if (imprimir_debug) debug_compiler();
        }
    }

    liberar_tabela();
    liberar_ast(raiz);
    if (yyin) fclose(yyin);
    return (resultado != 0 || erros_lexicos != 0 || erros_semanticos != 0)
        ? EXIT_FAILURE : EXIT_SUCCESS;
}
