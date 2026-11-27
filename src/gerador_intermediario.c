#include "gerador_intermediario.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

typedef struct Gerador {
    FILE *saida;
    size_t proximo_rotulo;
} Gerador;

static int eh(const NoAST *no, const char *texto) {
    return no != NULL && !no->identificador && strcmp(no->texto, texto) == 0;
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

static void desvio(Gerador *g, const char *opcode, size_t rotulo) {
    if (g->saida) fprintf(g->saida, "%s L%zu\n", opcode, rotulo);
}

static int gerar_expressao(Gerador *g, const NoAST *no);

static int gerar_logica(Gerador *g, const NoAST *no, const NoAST *a, const NoAST *b) {
    if (b == NULL || irmao(b) != NULL) return 0;
    int conjuncao = eh(no, "&&");
    size_t alternativo = g->proximo_rotulo++;
    size_t fim = g->proximo_rotulo++;
    if (!gerar_expressao(g, a)) return 0;
    if (!conjuncao) instrucao(g, "NOT");
    desvio(g, "JUMP_IF_FALSE", alternativo);
    if (!gerar_expressao(g, b)) return 0;
    desvio(g, "JUMP", fim);
    desvio(g, "LABEL", alternativo);
    constante(g, conjuncao ? "false" : "true");
    desvio(g, "LABEL", fim);
    return 1;
}

static int gerar_expressao(Gerador *g, const NoAST *no) {
    if (no == NULL || no->tipo == NULL) return 0;
    if (no->identificador) return variavel(g, "LOAD", no);
    if (no->inteiro_minimo) {
        constante(g, "-2147483648");
        return 1;
    }

    const NoAST *a = no->filho;
    const NoAST *b = irmao(a);
    if (a == NULL) {
        constante(g, no->texto);
        return 1;
    }

    if (eh(no, "&&") || eh(no, "||")) return gerar_logica(g, no, a, b);

    if (eh(no, "Math.sqrt") || eh(no, "Math.pow")) {
        int potencia = eh(no, "Math.pow");
        if ((b != NULL) != potencia || irmao(b) != NULL || !gerar_expressao(g, a))
            return 0;
        if (tipo_eh(a, "int")) instrucao(g, "I2F");
        if (potencia) {
            if (!gerar_expressao(g, b)) return 0;
            if (tipo_eh(b, "int")) instrucao(g, "I2F");
        }
        instrucao(g, potencia ? "POW" : "SQRT");
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

/* Cada chamada preserva o destino do contexto externo na pilha de C. */
typedef struct ContextoBreak {
    size_t fim;
} ContextoBreak;

static int gerar_bloco(Gerador *g, const NoAST *bloco, const ContextoBreak *saida);
static int gerar_comando(Gerador *g, const NoAST *no, const ContextoBreak *saida);

static int gerar_if(Gerador *g, const NoAST *condicao, const ContextoBreak *saida) {
    const NoAST *verdadeiro = irmao(condicao);
    const NoAST *falso = irmao(verdadeiro);
    size_t alternativo = g->proximo_rotulo++;
    size_t fim = falso != NULL ? g->proximo_rotulo++ : alternativo;
    if (!gerar_expressao(g, condicao)) return 0;
    desvio(g, "JUMP_IF_FALSE", alternativo);
    if (!gerar_bloco(g, verdadeiro, saida)) return 0;
    if (falso != NULL) {
        desvio(g, "JUMP", fim);
        desvio(g, "LABEL", alternativo);
        if (!gerar_bloco(g, falso, saida)) return 0;
    }
    desvio(g, "LABEL", fim);
    return 1;
}

static int gerar_laco(Gerador *g, const NoAST *no) {
    const NoAST *condicao = no->filho;
    const NoAST *atualizacao = NULL;
    if (eh(no, "For")) {
        if (!gerar_comando(g, no->filho, NULL)) return 0;
        condicao = irmao(no->filho);
        atualizacao = irmao(condicao);
        if (atualizacao == NULL) return 0;
    }
    const NoAST *corpo = atualizacao != NULL ? irmao(atualizacao) : irmao(condicao);
    size_t inicio = g->proximo_rotulo++;
    ContextoBreak saida = {g->proximo_rotulo++};
    desvio(g, "LABEL", inicio);
    if (!eh(condicao, "Vazio")) {
        if (!gerar_expressao(g, condicao)) return 0;
        desvio(g, "JUMP_IF_FALSE", saida.fim);
    }
    if (!gerar_bloco(g, corpo, &saida)) return 0;
    if (atualizacao != NULL && !eh(atualizacao, "Vazio")) {
        desvio(g, "LABEL", g->proximo_rotulo++);
        if (!gerar_comando(g, atualizacao, NULL)) return 0;
    }
    desvio(g, "JUMP", inicio);
    desvio(g, "LABEL", saida.fim);
    return 1;
}

static void temporario(Gerador *g, const char *opcode, size_t id) {
    if (g->saida) fprintf(g->saida, "%s @tmp%zu\n", opcode, id);
}

static int gerar_switch(Gerador *g, const NoAST *seletor) {
    const NoAST *lista = irmao(seletor);
    if (!eh(lista, "Casos") || !gerar_expressao(g, seletor)) return 0;
    ContextoBreak saida = {g->proximo_rotulo++};
    temporario(g, "STORE", saida.fim);

    size_t primeiro = g->proximo_rotulo;
    for (const NoAST *caso = lista->filho; caso != NULL; caso = caso->irmao)
        g->proximo_rotulo++;

    size_t destino_padrao = saida.fim;
    size_t rotulo = primeiro;
    for (const NoAST *caso = lista->filho; caso != NULL; caso = caso->irmao, rotulo++) {
        if (eh(caso, "Default")) {
            destino_padrao = rotulo;
        } else if (eh(caso, "Case") && tipo_eh(caso, "int") && caso->filho != NULL) {
            char valor[sizeof(int32_t) * 3 + 2];
            snprintf(valor, sizeof valor, "%" PRId32, caso->valor_caso);
            temporario(g, "LOAD", saida.fim);
            constante(g, valor);
            instrucao(g, "NE");
            desvio(g, "JUMP_IF_FALSE", rotulo);
        } else {
            return 0;
        }
    }
    desvio(g, "JUMP", destino_padrao);

    rotulo = primeiro;
    for (const NoAST *caso = lista->filho; caso != NULL; caso = caso->irmao, rotulo++) {
        desvio(g, "LABEL", rotulo);
        const NoAST *corpo = eh(caso, "Default") ? caso->filho : irmao(caso->filho);
        if (!gerar_bloco(g, corpo, &saida)) return 0;
    }
    desvio(g, "LABEL", saida.fim);
    return 1;
}

static int gerar_comando(Gerador *g, const NoAST *no, const ContextoBreak *saida) {
    if (no == NULL) return 0;
    const NoAST *a = no->filho;
    if (eh(no, "Bloco")) return gerar_bloco(g, no, saida);
    if (eh(no, "If")) return gerar_if(g, a, saida);
    if (eh(no, "While") || eh(no, "For")) return gerar_laco(g, no);
    if (eh(no, "Switch")) return gerar_switch(g, a);
    if (eh(no, "Print") || eh(no, "Println")) {
        if (!gerar_expressao(g, a)) return 0;
        instrucao(g, eh(no, "Print") ? "PRINT" : "PRINTLN");
        return 1;
    }
    if (eh(no, "Vazio")) return 1;
    if (eh(no, "Break") && saida != NULL) {
        desvio(g, "JUMP", saida->fim);
        return 1;
    }
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

static int gerar_bloco(Gerador *g, const NoAST *bloco, const ContextoBreak *saida) {
    if (!eh(bloco, "Bloco")) return 0;
    for (const NoAST *no = bloco->filho; no != NULL; no = no->irmao) {
        if (!gerar_comando(g, no, saida)) return 0;
    }
    return 1;
}

int imprimir_codigo_intermediario(const NoAST *bloco) {
    Gerador validacao = {0};
    if (!gerar_bloco(&validacao, bloco, NULL)) {
        fprintf(stderr, "Geracao intermediaria: instrucao ou expressao nao suportada.\n");
        return 0;
    }
    Gerador emissao = {.saida = stdout};
    return gerar_bloco(&emissao, bloco, NULL) && fflush(stdout) == 0;
}
