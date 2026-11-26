#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    no->identificador = 0;
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

NoAST *criar_identificador(char *texto) {
    NoAST *no = criar_folha(texto);
    no->identificador = 1;
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

static const char *instrucao_aritmetica(const char *operador, int unario) {
    if (strcmp(operador, "-") == 0) return unario ? "NEG" : "SUB";
    if (unario) return NULL;
    if (strcmp(operador, "+") == 0) return "ADD";
    if (strcmp(operador, "*") == 0) return "MUL";
    if (strcmp(operador, "/") == 0) return "DIV";
    if (strcmp(operador, "%") == 0) return "MOD";
    return NULL;
}

static int gerar_expressao_inteira(const NoAST *no, int emitir) {
    if (no == NULL) return 0;

    const NoAST *esquerda = no->filho;
    if (esquerda == NULL) {
        if (no->identificador || no->texto[0] == '\0'
                || no->texto[strspn(no->texto, "0123456789")] != '\0') {
            return 0;
        }
        if (emitir) printf("LOAD_CONST %s\n", no->texto);
        return 1;
    }

    const NoAST *direita = esquerda->irmao;
    const char *instrucao = instrucao_aritmetica(no->texto, direita == NULL);
    if (instrucao == NULL || !gerar_expressao_inteira(esquerda, emitir)
            || (direita != NULL && !gerar_expressao_inteira(direita, emitir))) {
        return 0;
    }

    if (emitir) puts(instrucao);
    return 1;
}

static int gerar_declaracoes_inteiras(const NoAST *bloco, int emitir) {
    for (const NoAST *no = bloco->filho; no != NULL; no = no->irmao) {
        if (strcmp(no->texto, "Declaracao") != 0) return 0;

        const NoAST *tipo = no->filho;
        const NoAST *nome = tipo->irmao;
        if (strcmp(tipo->texto, "int") != 0
                || !gerar_expressao_inteira(nome->irmao, emitir)) {
            return 0;
        }

        if (emitir) printf("STORE %s\n", nome->texto);
    }

    return 1;
}

int imprimir_codigo_intermediario(const NoAST *bloco) {
    if (!gerar_declaracoes_inteiras(bloco, 0)) {
        fprintf(stderr,
                "Geracao intermediaria: esperadas declaracoes int com expressoes de literais inteiros.\n");
        return 0;
    }

    return gerar_declaracoes_inteiras(bloco, 1);
}
