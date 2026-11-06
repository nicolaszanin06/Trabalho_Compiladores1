%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* A tabela de simbolos e a raiz usada pelas acoes da gramatica. */
%code {
/* Uma entrada para cada declaracao encontrada no programa. */
typedef struct Simbolo {
    char *simbolo;
    char *tipo;
    char *valor;
    const char *escopo;
    struct Simbolo *proximo;
} Simbolo;

void inserir_simbolo(NoAST *declaracao, int nivel);
void montar_tabela(NoAST *no, int nivel);
void imprimir_tabela(void);
void liberar_tabela(void);

NoAST *raiz = NULL;
Simbolo *tabela_simbolos = NULL;
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
      tipo IDENTIFIER ASSIGN expressao { $$ = montar_no("Declaracao", $1, criar_folha($2), $4); }
    | tipo IDENTIFIER { $$ = montar_no("Declaracao", $1, criar_folha($2), NULL); }
    ;

atribuicao:
      IDENTIFIER operador_atribuicao expressao {
          adicionar_filho($2, criar_folha($1));
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
    | IDENTIFIER incrementacao { adicionar_filho($2, criar_folha($1)); $$ = $2; }
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
    
    | IDENTIFIER { $$ = criar_folha($1); }
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

void inserir_simbolo(NoAST *declaracao, int nivel) {
    NoAST *tipo = declaracao->filho;
    NoAST *nome = tipo->irmao;
    Simbolo *atual;
    Simbolo *novo = malloc(sizeof(Simbolo));
    if (novo == NULL) {
        fprintf(stderr, "Memoria insuficiente para a tabela de simbolos.\n");
        exit(EXIT_FAILURE);
    }
    novo->simbolo = strdup(nome->texto);
    novo->tipo = strdup(tipo->texto);
    novo->valor = texto_expressao(nome->irmao);
    novo->escopo = nivel == 0 ? "global" : "local";
    novo->proximo = NULL;
    if (novo->simbolo == NULL || novo->tipo == NULL || novo->valor == NULL) {
        fprintf(stderr, "Memoria insuficiente para a tabela de simbolos.\n");
        exit(EXIT_FAILURE);
    }

    if (tabela_simbolos == NULL) {
        tabela_simbolos = novo;
    } else {
        atual = tabela_simbolos;
        while (atual->proximo != NULL) atual = atual->proximo;
        atual->proximo = novo;
    }
}

/* O bloco principal tem nivel 0. Qualquer bloco interno e local.
 * Cada declaracao ocupa uma linha, inclusive nomes iguais em blocos diferentes.
 * Visitamos apenas comandos e blocos, sem confundir identificadores com comandos. */
void montar_tabela(NoAST *bloco, int nivel) {
    NoAST *comando = bloco->filho;
    while (comando != NULL) {
        if (strcmp(comando->texto, "Declaracao") == 0) {
            inserir_simbolo(comando, nivel);
        } else if (strcmp(comando->texto, "Bloco") == 0) {
            montar_tabela(comando, nivel + 1);
        } else if (strcmp(comando->texto, "If") == 0 ||
                   strcmp(comando->texto, "While") == 0) {
            /* O primeiro filho e a condicao; os demais sao os blocos. */
            NoAST *corpo = comando->filho->irmao;
            while (corpo != NULL) {
                montar_tabela(corpo, nivel + 1);
                corpo = corpo->irmao;
            }
        } else if (strcmp(comando->texto, "For") == 0) {
            NoAST *inicializacao = comando->filho;
            NoAST *corpo = inicializacao->irmao->irmao->irmao;
            if (strcmp(inicializacao->texto, "Declaracao") == 0) {
                inserir_simbolo(inicializacao, nivel + 1);
            }
            montar_tabela(corpo, nivel + 2);
        } else if (strcmp(comando->texto, "Switch") == 0) {
            NoAST *caso = comando->filho->irmao->filho;
            while (caso != NULL) {
                NoAST *corpo = caso->filho;
                if (strcmp(caso->texto, "Case") == 0) corpo = corpo->irmao;
                montar_tabela(corpo, nivel + 1);
                caso = caso->irmao;
            }
        }
        comando = comando->irmao;
    }
}

void imprimir_tabela(void) {
    Simbolo *atual = tabela_simbolos;
    printf("\nTabela de simbolos (valor de inicializacao):\n");
    printf("%-20s | %-10s | %-30s | %s\n", "Simbolo", "Tipo", "Valor", "Escopo");
    while (atual != NULL) {
        printf("%-20s | %-10s | %-30s | %s\n",
               atual->simbolo, atual->tipo, atual->valor, atual->escopo);
        atual = atual->proximo;
    }
}

void liberar_tabela(void) {
    while (tabela_simbolos != NULL) {
        Simbolo *proximo = tabela_simbolos->proximo;
        free(tabela_simbolos->simbolo);
        free(tabela_simbolos->tipo);
        free(tabela_simbolos->valor);
        free(tabela_simbolos);
        tabela_simbolos = proximo;
    }
}

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

    int resultado = yyparse();
    if (resultado == 0 && erros_lexicos == 0) {
        printf("Analise concluida com sucesso! Nenhum erro sintatico encontrado.\n");
        printf("AST:\n");
        imprimir_ast(raiz, 0);
        montar_tabela(raiz, 0);
        imprimir_tabela();
    }

    liberar_tabela();
    liberar_ast(raiz);
    if (yyin) fclose(yyin);
    return (resultado != 0 || erros_lexicos != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
