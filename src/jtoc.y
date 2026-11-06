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
    int id_escopo;
    struct Simbolo *proximo;
} Simbolo;

/* Os pais ficam na pilha durante a visita da AST; a tabela guarda apenas o id. */
typedef struct Escopo {
    int id;
    struct Escopo *pai;
} Escopo;

Simbolo *buscar_no_escopo(const char *nome, int id_escopo);
Simbolo *buscar_simbolo(const char *nome, Escopo *escopo);
void inserir_simbolo(NoAST *declaracao, Escopo *escopo);
void verificar_expressao(NoAST *no, Escopo *escopo);
void analisar_comando(NoAST *comando, Escopo *escopo);
void analisar_comandos(NoAST *bloco, Escopo *escopo);
void montar_tabela(NoAST *bloco, Escopo *pai);
void imprimir_tabela(void);
void liberar_tabela(void);

NoAST *raiz = NULL;
Simbolo *tabela_simbolos = NULL;
int proximo_escopo = 0;
int erros_semanticos = 0;
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

/* Escopos diferentes podem ter a mesma profundidade, mas nunca o mesmo id. */
Simbolo *buscar_no_escopo(const char *nome, int id_escopo) {
    Simbolo *atual = tabela_simbolos;
    while (atual != NULL) {
        if (atual->id_escopo == id_escopo && strcmp(atual->simbolo, nome) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

Simbolo *buscar_simbolo(const char *nome, Escopo *escopo) {
    while (escopo != NULL) {
        Simbolo *simbolo = buscar_no_escopo(nome, escopo->id);
        if (simbolo != NULL) return simbolo;
        escopo = escopo->pai;
    }
    return NULL;
}

void inserir_simbolo(NoAST *declaracao, Escopo *escopo) {
    NoAST *tipo = declaracao->filho;
    NoAST *nome = tipo->irmao;
    Simbolo *atual;
    Simbolo *novo;
    if (buscar_no_escopo(nome->texto, escopo->id) != NULL) {
        fprintf(stderr, "Erro semântico: Variável já declarada '%s'.\n", nome->texto);
        erros_semanticos++;
        return;
    }
    novo = malloc(sizeof(Simbolo));
    if (novo == NULL) {
        fprintf(stderr, "Memoria insuficiente para a tabela de simbolos.\n");
        exit(EXIT_FAILURE);
    }
    novo->simbolo = strdup(nome->texto);
    novo->tipo = strdup(tipo->texto);
    novo->valor = texto_expressao(nome->irmao);
    novo->escopo = escopo->pai == NULL ? "global" : "local";
    novo->id_escopo = escopo->id;
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

/* Visita somente a expressao recebida; seus irmaos pertencem ao comando pai. */
void verificar_expressao(NoAST *no, Escopo *escopo) {
    NoAST *filho;
    if (no == NULL) return;
    if (no->identificador && buscar_simbolo(no->texto, escopo) == NULL) {
        fprintf(stderr, "Erro semântico: Variável não declarada '%s'.\n", no->texto);
        erros_semanticos++;
    }
    filho = no->filho;
    while (filho != NULL) {
        verificar_expressao(filho, escopo);
        filho = filho->irmao;
    }
}

void analisar_comando(NoAST *comando, Escopo *escopo) {
    if (strcmp(comando->texto, "Declaracao") == 0) {
        inserir_simbolo(comando, escopo);
        verificar_expressao(comando->filho->irmao->irmao, escopo);
    } else if (strcmp(comando->texto, "Bloco") == 0) {
        montar_tabela(comando, escopo);
    } else if (strcmp(comando->texto, "If") == 0 ||
               strcmp(comando->texto, "While") == 0) {
        NoAST *condicao = comando->filho;
        NoAST *corpo = condicao->irmao;
        verificar_expressao(condicao, escopo);
        while (corpo != NULL) {
            montar_tabela(corpo, escopo);
            corpo = corpo->irmao;
        }
    } else if (strcmp(comando->texto, "For") == 0) {
        Escopo escopo_for = {proximo_escopo++, escopo};
        NoAST *inicializacao = comando->filho;
        NoAST *condicao = inicializacao->irmao;
        NoAST *atualizacao = condicao->irmao;
        NoAST *corpo = atualizacao->irmao;
        analisar_comando(inicializacao, &escopo_for);
        verificar_expressao(condicao, &escopo_for);
        analisar_comando(atualizacao, &escopo_for);
        montar_tabela(corpo, &escopo_for);
    } else if (strcmp(comando->texto, "Switch") == 0) {
        Escopo escopo_switch = {proximo_escopo++, escopo};
        NoAST *expressao = comando->filho;
        NoAST *caso = expressao->irmao->filho;
        verificar_expressao(expressao, escopo);
        while (caso != NULL) {
            NoAST *corpo = caso->filho;
            if (strcmp(caso->texto, "Case") == 0) {
                verificar_expressao(corpo, &escopo_switch);
                corpo = corpo->irmao;
            }
            /* Case nao abre escopo; apenas chaves explicitas abrem outro. */
            analisar_comandos(corpo, &escopo_switch);
            caso = caso->irmao;
        }
    } else {
        /* Atribuicoes, incrementos e impressao: todos os usos sao filhos. */
        verificar_expressao(comando, escopo);
    }
}

void analisar_comandos(NoAST *bloco, Escopo *escopo) {
    NoAST *comando = bloco->filho;
    while (comando != NULL) {
        analisar_comando(comando, escopo);
        comando = comando->irmao;
    }
}

void montar_tabela(NoAST *bloco, Escopo *pai) {
    Escopo escopo = {proximo_escopo++, pai};
    analisar_comandos(bloco, &escopo);
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
        montar_tabela(raiz, NULL);
        if (erros_semanticos == 0) {
            printf("Analise concluida com sucesso! Nenhum erro sintatico encontrado.\n");
            printf("AST:\n");
            imprimir_ast(raiz, 0);
            imprimir_tabela();
        }
    }

    liberar_tabela();
    liberar_ast(raiz);
    if (yyin) fclose(yyin);
    return (resultado != 0 || erros_lexicos != 0 || erros_semanticos != 0)
        ? EXIT_FAILURE : EXIT_SUCCESS;
}
