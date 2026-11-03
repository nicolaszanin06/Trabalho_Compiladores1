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

/* O scanner so precisa conhecer o nome do tipo usado no yylval. */
%code requires {
typedef struct NoAST NoAST;
}

/* A estrutura e as funcoes da AST pertencem ao analisador sintatico. */
%code {
struct NoAST {
    char *texto;
    NoAST *filho;
    NoAST *irmao;
};

/* Uma entrada para cada declaracao encontrada no programa. */
typedef struct Simbolo {
    char *simbolo;
    char *tipo;
    char *valor;
    const char *escopo;
    struct Simbolo *proximo;
} Simbolo;

NoAST *criar_no(const char *texto);
NoAST *criar_folha(char *texto);
NoAST *montar_no(const char *texto, NoAST *a, NoAST *b, NoAST *c);
void adicionar_filho(NoAST *pai, NoAST *filho);
void imprimir_ast(NoAST *no, int nivel);
void liberar_ast(NoAST *no);
char *texto_expressao(NoAST *no);
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

/* A raiz so e guardada quando toda a entrada foi reconhecida. */
inicio:
      programa { raiz = $1; }
    ;

programa:
      %empty { $$ = criar_no("Bloco"); }
    | programa comando { adicionar_filho($1, $2); $$ = $1; }
    ;

comando:
     tipo IDENTIFIER ASSIGN expressao SEMI { $$ = montar_no("Declaracao", $1, criar_folha($2), $4); }
    | tipo IDENTIFIER SEMI { $$ = montar_no("Declaracao", $1, criar_folha($2), NULL); }
    | IDENTIFIER ASSIGN expressao SEMI { $$ = montar_no("=", criar_folha($1), $3, NULL); }
    | IF LPAREN expressao RPAREN LBRACE programa RBRACE ELSE LBRACE programa RBRACE { $$ = montar_no("If", $3, $6, $10); }
    | IF LPAREN expressao RPAREN LBRACE programa RBRACE { $$ = montar_no("If", $3, $6, NULL); }
    | WHILE LPAREN expressao RPAREN LBRACE programa RBRACE { $$ = montar_no("While", $3, $6, NULL); }
    | IDENTIFIER incrementacao SEMI { adicionar_filho($2, criar_folha($1)); $$ = $2; }
    | PRINTLN LPAREN expressao RPAREN SEMI { $$ = montar_no("Println", $3, NULL, NULL); }
    | PRINT LPAREN expressao RPAREN SEMI { $$ = montar_no("Print", $3, NULL, NULL); }
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

/* Cada no guarda um texto, seu primeiro filho e o proximo irmao.
 * Assim, um bloco pode ter quantos comandos forem necessarios. */
NoAST *criar_no(const char *texto) {
    NoAST *no = malloc(sizeof(NoAST));
    if (no == NULL) {
        fprintf(stderr, "Memoria insuficiente para a AST.\n");
        exit(EXIT_FAILURE);
    }
    no->texto = strdup(texto);
    if (no->texto == NULL) {
        free(no);
        fprintf(stderr, "Memoria insuficiente para a AST.\n");
        exit(EXIT_FAILURE);
    }
    no->filho = NULL;
    no->irmao = NULL;
    return no;
}

void adicionar_filho(NoAST *pai, NoAST *filho) {
    NoAST *atual;
    if (filho == NULL) return;
    if (pai->filho == NULL) {
        pai->filho = filho;
        return;
    }
    atual = pai->filho;
    while (atual->irmao != NULL) atual = atual->irmao;
    atual->irmao = filho;
}

/* Atalho para criar comandos e operadores com ate tres filhos. */
NoAST *montar_no(const char *texto, NoAST *a, NoAST *b, NoAST *c) {
    NoAST *no = criar_no(texto);
    adicionar_filho(no, a);
    adicionar_filho(no, b);
    adicionar_filho(no, c);
    return no;
}

/* O texto recebido pertence ao token e pode ser liberado apos a copia.
 * Guardamos o lexema inteiro para preservar aspas e escapes de literais. */
NoAST *criar_folha(char *texto) {
    NoAST *no = criar_no(texto);
    free(texto);
    return no;
}

void imprimir_ast(NoAST *no, int nivel) {
    while (no != NULL) {
        printf("%*s%s\n", nivel * 2, "", no->texto);
        imprimir_ast(no->filho, nivel + 1);
        no = no->irmao;
    }
}

void liberar_ast(NoAST *no) {
    while (no != NULL) {
        NoAST *proximo = no->irmao;
        liberar_ast(no->filho);
        free(no->texto);
        free(no);
        no = proximo;
    }
}

/* Converte a expressao da AST em texto, sem executar o programa.
 * O valor registrado e o da inicializacao, nao o resultado em tempo de execucao. */
char *texto_expressao(NoAST *no) {
    char *esquerda, *direita, *texto;
    size_t tamanho;
    if (no == NULL) return strdup("nao inicializado");
    if (no->filho == NULL) return strdup(no->texto);

    esquerda = texto_expressao(no->filho);
    direita = no->filho->irmao != NULL
        ? texto_expressao(no->filho->irmao) : NULL;
    tamanho = strlen(no->texto) + strlen(esquerda) + 6;
    if (direita != NULL) tamanho += strlen(direita);
    texto = malloc(tamanho);
    if (texto == NULL) {
        fprintf(stderr, "Memoria insuficiente para a tabela de simbolos.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(no->texto, "Math.pow") == 0) {
        snprintf(texto, tamanho, "Math.pow(%s, %s)", esquerda, direita);
    } else if (strcmp(no->texto, "Math.sqrt") == 0) {
        snprintf(texto, tamanho, "Math.sqrt(%s)", esquerda);
    } else if (direita == NULL) {
        snprintf(texto, tamanho, "(%s%s)", no->texto, esquerda);
    } else {
        snprintf(texto, tamanho, "(%s %s %s)", esquerda, no->texto, direita);
    }
    free(esquerda);
    free(direita);
    return texto;
}

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
        } else if (strcmp(comando->texto, "If") == 0 ||
                   strcmp(comando->texto, "While") == 0) {
            /* O primeiro filho e a condicao; os demais sao os blocos. */
            NoAST *corpo = comando->filho->irmao;
            while (corpo != NULL) {
                montar_tabela(corpo, nivel + 1);
                corpo = corpo->irmao;
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
