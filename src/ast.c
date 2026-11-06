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
