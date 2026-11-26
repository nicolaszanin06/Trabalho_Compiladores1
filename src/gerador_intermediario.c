#include "gerador_intermediario.h"

#include <stdio.h>
#include <string.h>

/* Retorna a instrucao correspondente ao operador. */
static const char *instrucao_aritmetica(
    const char *operador,
    int unario
) {
    if (strcmp(operador, "-") == 0)
        return unario ? "NEG" : "SUB";

    if (unario)
        return NULL;

    if (strcmp(operador, "+") == 0)
        return "ADD";

    if (strcmp(operador, "*") == 0)
        return "MUL";

    if (strcmp(operador, "/") == 0)
        return "DIV";

    if (strcmp(operador, "%") == 0)
        return "MOD";

    return NULL;
}

/* Valida ou gera uma expressao de literais inteiros. */
static int gerar_expressao_inteira(
    const NoAST *no,
    int emitir
) {
    if (no == NULL)
        return 0;

    const NoAST *esquerda = no->filho;

    /* Caso base: literal inteiro. */
    if (esquerda == NULL) {
        if (no->identificador ||
            no->texto[0] == '\0' ||
            no->texto[strspn(
                no->texto, "0123456789"
            )] != '\0') {
            return 0;
        }

        if (emitir)
            printf("LOAD_CONST %s\n", no->texto);

        return 1;
    }

    const NoAST *direita = esquerda->irmao;

    const char *instrucao = instrucao_aritmetica(
        no->texto,
        direita == NULL
    );

    if (instrucao == NULL)
        return 0;

    if (direita != NULL && direita->irmao != NULL)
        return 0;

    /* Primeiro, gera o operando esquerdo. */
    if (!gerar_expressao_inteira(esquerda, emitir))
        return 0;

    /* Depois, gera o operando direito, se existir. */
    if (direita != NULL &&
        !gerar_expressao_inteira(direita, emitir)) {
        return 0;
    }

    /* Por ultimo, gera a operacao. */
    if (emitir)
        puts(instrucao);

    return 1;
}

/* Percorre as declaracoes presentes no bloco. */
static int gerar_declaracoes_inteiras(
    const NoAST *bloco,
    int emitir
) {
    if (bloco == NULL)
        return 0;

    for (
        const NoAST *no = bloco->filho;
        no != NULL;
        no = no->irmao
    ) {
        if (strcmp(no->texto, "Declaracao") != 0)
            return 0;

        const NoAST *tipo = no->filho;

        if (tipo == NULL)
            return 0;

        const NoAST *nome = tipo->irmao;

        if (nome == NULL)
            return 0;

        const NoAST *expressao = nome->irmao;

        if (strcmp(tipo->texto, "int") != 0)
            return 0;

        if (!gerar_expressao_inteira(
                expressao, emitir
            )) {
            return 0;
        }

        if (emitir)
            printf("STORE %s\n", nome->texto);
    }

    return 1;
}

/* Interface publica do gerador. */
int imprimir_codigo_intermediario(
    const NoAST *bloco
) {
    /* Primeira passagem: somente validacao. */
    if (!gerar_declaracoes_inteiras(bloco, 0)) {
        fprintf(
            stderr,
            "Geracao intermediaria: esperadas "
            "declaracoes int com expressoes "
            "de literais inteiros.\n"
        );

        return 0;
    }

    /* Segunda passagem: emissao das instrucoes. */
    return gerar_declaracoes_inteiras(bloco, 1);
}