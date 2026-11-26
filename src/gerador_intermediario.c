#include "gerador_intermediario.h"

#include <stdio.h>
#include <string.h>

typedef struct Gerador {
    FILE *saida;
} Gerador;

static int eh(const NoAST *no, const char *texto) {
    return no != NULL && strcmp(no->texto, texto) == 0;
}

static const NoAST *irmao(const NoAST *no) {
    return no != NULL ? no->irmao : NULL;
}

static int tipo_eh(const NoAST *no, const char *tipo) {
    return no != NULL && no->tipo != NULL && strcmp(no->tipo, tipo) == 0;
}

static const char *instrucao_operador(const char *operador, int unario) {
    static const struct { const char *operador; const char *instrucao; } operacoes[] = {
        {"+", "ADD"}, {"-", "SUB"}, {"*", "MUL"}, {"/", "DIV"}, {"%", "MOD"},
        {"+=", "ADD"}, {"-=", "SUB"}, {"*=", "MUL"}, {"/=", "DIV"},
        {"++", "ADD"}, {"--", "SUB"},
        {"==", "EQ"}, {"!=", "NE"}, {"<", "LT"}, {"<=", "LE"}, {">", "GT"}, {">=", "GE"}
    };
    if (unario) {
        if (strcmp(operador, "-") == 0) return "NEG";
        if (strcmp(operador, "!") == 0) return "NOT";
        return NULL;
    }
    for (size_t i = 0; i < sizeof operacoes / sizeof operacoes[0]; i++) {
        if (strcmp(operador, operacoes[i].operador) == 0) return operacoes[i].instrucao;
    }
    return NULL;
}

static void instrucao(Gerador *g, const char *opcode) {
    if (g->saida) fprintf(g->saida, "%s\n", opcode);
}

static void constante(Gerador *g, const char *lexema) {
    if (g->saida) fprintf(g->saida, "LOAD_CONST %s\n", lexema);
}

static int variavel(Gerador *g, const char *opcode, const NoAST *nome) {
    if (nome == NULL || !nome->identificador || nome->id_escopo < 0 || nome->tipo == NULL)
        return 0;
    if (g->saida) {
        if (nome->id_escopo == 0) fprintf(g->saida, "%s %s\n", opcode, nome->texto);
        else fprintf(g->saida, "%s @%d:%s\n", opcode, nome->id_escopo, nome->texto);
    }
    return 1;
}

static int gerar_expressao(Gerador *g, const NoAST *no) {
    if (no == NULL || no->tipo == NULL) return 0;
    if (no->identificador) return variavel(g, "LOAD", no);

    const NoAST *a = no->filho;
    const NoAST *b = irmao(a);
    if (a == NULL) {
        constante(g, no->texto);
        return 1;
    }

    const char *opcode = instrucao_operador(no->texto, b == NULL);
    if (opcode == NULL || irmao(b) != NULL || !gerar_expressao(g, a)) return 0;
    if (tipo_eh(a, "int") && tipo_eh(b, "float")) instrucao(g, "I2F");
    if (b != NULL && !gerar_expressao(g, b)) return 0;
    if (tipo_eh(a, "float") && tipo_eh(b, "int")) instrucao(g, "I2F");
    instrucao(g, opcode);
    return 1;
}

static int gerar_bloco(Gerador *g, const NoAST *bloco);

static int gerar_comando(Gerador *g, const NoAST *no) {
    if (no == NULL) return 0;
    const NoAST *a = no->filho;
    if (eh(no, "Bloco")) return gerar_bloco(g, no);
    if (eh(no, "Declaracao") || eh(no, "=")) {
        const NoAST *nome = eh(no, "Declaracao") ? irmao(a) : a;
        const NoAST *valor = irmao(nome);
        if (valor == NULL) return nome != NULL && eh(no, "Declaracao");
        if (!gerar_expressao(g, valor)) return 0;
        if (tipo_eh(nome, "float") && tipo_eh(valor, "int")) instrucao(g, "I2F");
        return variavel(g, "STORE", nome);
    }

    if (eh(no, "+=") || eh(no, "-=") || eh(no, "*=") || eh(no, "/=")
            || eh(no, "++") || eh(no, "--")) {
        if (!variavel(g, "LOAD", a)) return 0;
        const NoAST *valor = irmao(a);
        if (eh(no, "++") || eh(no, "--")) constante(g, tipo_eh(a, "float") ? "1.0" : "1");
        else {
            if (!gerar_expressao(g, valor)) return 0;
            if (tipo_eh(a, "float") && tipo_eh(valor, "int")) instrucao(g, "I2F");
        }
        instrucao(g, instrucao_operador(no->texto, 0));
        return variavel(g, "STORE", a);
    }
    return 0;
}

static int gerar_bloco(Gerador *g, const NoAST *bloco) {
    if (!eh(bloco, "Bloco")) return 0;
    for (const NoAST *no = bloco->filho; no != NULL; no = no->irmao) {
        if (!gerar_comando(g, no)) return 0;
    }
    return 1;
}

int imprimir_codigo_intermediario(const NoAST *bloco) {
    Gerador validacao = {NULL};
    if (!gerar_bloco(&validacao, bloco)) {
        fprintf(stderr, "Geracao intermediaria: instrucao ou expressao nao suportada.\n");
        return 0;
    }
    Gerador emissao = {stdout};
    return gerar_bloco(&emissao, bloco) && fflush(stdout) == 0;
}
