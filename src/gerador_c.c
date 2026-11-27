#include "gerador_c.h"

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LiteralString {
    unsigned char *bytes;
    size_t tamanho;
} LiteralString;

typedef struct GeradorC {
    FILE *saida;
    LiteralString *strings;
    size_t quantidade_strings;
    size_t capacidade_strings;
    int indentacao;
    int precisa_math;
    int falha;
} GeradorC;

static int eh(const NoAST *no, const char *texto) {
    return no != NULL && !no->identificador && strcmp(no->texto, texto) == 0;
}

static const NoAST *irmao(const NoAST *no) {
    return no != NULL ? no->irmao : NULL;
}

static int tipo_eh(const NoAST *no, const char *tipo) {
    return no != NULL && no->tipo != NULL && strcmp(no->tipo, tipo) == 0;
}

static int escrever(GeradorC *g, const char *formato, ...) {
    va_list args;
    int resultado;

    if (g->falha) return 0;
    va_start(args, formato);
    resultado = vfprintf(g->saida, formato, args);
    va_end(args);

    if (resultado < 0) {
        g->falha = 1;
        return 0;
    }
    return 1;
}

static int indent(GeradorC *g) {
    for (int i = 0; i < g->indentacao; i++) {
        if (!escrever(g, "    ")) return 0;
    }
    return 1;
}

static int erro(GeradorC *g, const char *mensagem) {
    if (!g->falha) fprintf(stderr, "Geracao C: %s\n", mensagem);
    g->falha = 1;
    return 0;
}

static const char *tipo_c(const char *tipo) {
    if (tipo == NULL) return NULL;
    if (strcmp(tipo, "int") == 0) return "int";
    if (strcmp(tipo, "float") == 0) return "float";
    if (strcmp(tipo, "char") == 0) return "char";
    if (strcmp(tipo, "boolean") == 0) return "bool";
    if (strcmp(tipo, "String") == 0) return "const char *";
    return NULL;
}

static int nome_variavel(GeradorC *g, const NoAST *no) {
    if (no == NULL || !no->identificador || no->tipo == NULL || no->id_escopo < 0)
        return erro(g, "referencia de variavel sem identificador semantico valido.");
    return escrever(g, "jtoc_v%zu", no->id_simbolo);
}

/* Converte os escapes aceitos pelo scanner para os bytes que representam. */
static int decodificar_literal(const char *lexema, unsigned char **bytes, size_t *tamanho) {
    size_t comprimento;
    size_t usados = 0;
    unsigned char *resultado;
    char delimitador;

    if (lexema == NULL) return 0;
    comprimento = strlen(lexema);
    if (comprimento < 2) return 0;
    delimitador = lexema[0];
    if ((delimitador != '"' && delimitador != '\'') || lexema[comprimento - 1] != delimitador)
        return 0;

    resultado = malloc(comprimento);
    if (resultado == NULL) return 0;

    for (size_t i = 1; i + 1 < comprimento; i++) {
        unsigned char atual = (unsigned char)lexema[i];
        if (atual == '\\') {
            if (++i + 1 >= comprimento) {
                free(resultado);
                return 0;
            }
            switch (lexema[i]) {
                case 'b': atual = '\b'; break;
                case 't': atual = '\t'; break;
                case 'n': atual = '\n'; break;
                case 'f': atual = '\f'; break;
                case 'r': atual = '\r'; break;
                case '"': atual = '"'; break;
                case '\'': atual = '\''; break;
                case '\\': atual = '\\'; break;
                default:
                    free(resultado);
                    return 0;
            }
        }
        resultado[usados++] = atual;
    }

    *bytes = resultado;
    *tamanho = usados;
    return 1;
}

static int adicionar_string(GeradorC *g, const unsigned char *bytes, size_t tamanho) {
    for (size_t i = 0; i < g->quantidade_strings; i++) {
        if (g->strings[i].tamanho == tamanho
                && memcmp(g->strings[i].bytes, bytes, tamanho) == 0)
            return 1;
    }

    if (g->quantidade_strings == g->capacidade_strings) {
        size_t nova_capacidade = g->capacidade_strings == 0 ? 8 : g->capacidade_strings * 2;
        LiteralString *novas = realloc(g->strings, nova_capacidade * sizeof *novas);
        if (novas == NULL) return erro(g, "memoria insuficiente ao coletar literais String.");
        g->strings = novas;
        g->capacidade_strings = nova_capacidade;
    }

    unsigned char *copia = malloc(tamanho == 0 ? 1 : tamanho);
    if (copia == NULL) return erro(g, "memoria insuficiente ao copiar literal String.");
    if (tamanho != 0) memcpy(copia, bytes, tamanho);

    g->strings[g->quantidade_strings].bytes = copia;
    g->strings[g->quantidade_strings].tamanho = tamanho;
    g->quantidade_strings++;
    return 1;
}

static int coletar_recursos(GeradorC *g, const NoAST *no) {
    for (; no != NULL; no = no->irmao) {
        if (!no->identificador && no->texto != NULL && no->texto[0] == '"') {
            unsigned char *bytes = NULL;
            size_t tamanho = 0;
            if (!decodificar_literal(no->texto, &bytes, &tamanho))
                return erro(g, "literal String possui escape ou delimitacao invalida.");
            int ok = adicionar_string(g, bytes, tamanho);
            free(bytes);
            if (!ok) return 0;
        }

        if (!no->identificador && no->texto != NULL
                && (strcmp(no->texto, "Math.sqrt") == 0
                    || strcmp(no->texto, "Math.pow") == 0
                    || (strcmp(no->texto, "%") == 0 && tipo_eh(no, "float"))))
            g->precisa_math = 1;

        if (!coletar_recursos(g, no->filho)) return 0;
    }
    return 1;
}

static int indice_string(GeradorC *g, const char *lexema, size_t *indice) {
    unsigned char *bytes = NULL;
    size_t tamanho = 0;
    int encontrada = 0;

    if (!decodificar_literal(lexema, &bytes, &tamanho))
        return erro(g, "nao foi possivel decodificar literal String.");

    for (size_t i = 0; i < g->quantidade_strings; i++) {
        if (g->strings[i].tamanho == tamanho
                && memcmp(g->strings[i].bytes, bytes, tamanho) == 0) {
            *indice = i;
            encontrada = 1;
            break;
        }
    }
    free(bytes);
    if (!encontrada) return erro(g, "literal String nao encontrado na tabela de constantes.");
    return 1;
}

static int emitir_cabecalho(GeradorC *g) {
    if (!escrever(g, "#include <stdio.h>\n#include <stdbool.h>\n"
                     "#include <stdint.h>\n#include <limits.h>\n#include <stdlib.h>\n"))
        return 0;
    if (g->precisa_math && !escrever(g, "#include <math.h>\n")) return 0;
    return escrever(g,
        "\n_Static_assert(INT_MAX == INT32_MAX && INT_MIN == INT32_MIN,\n"
        "               \"O programa requer int de 32 bits\");\n\n");
}

static int emitir_auxiliares(GeradorC *g) {
    return escrever(g,
        "/* Aritmetica inteira com retorno circular de 32 bits. */\n"
        "static inline int32_t jtoc_i32(int64_t valor) {\n"
        "    uint32_t bits = (uint32_t)valor;\n"
        "    return bits <= INT32_MAX\n"
        "        ? (int32_t)bits\n"
        "        : (int32_t)((int64_t)bits - INT64_C(4294967296));\n"
        "}\n\n"
        "static inline int32_t jtoc_iadd(int32_t a, int32_t b) {\n"
        "    return jtoc_i32((int64_t)a + (int64_t)b);\n"
        "}\n"
        "static inline int32_t jtoc_isub(int32_t a, int32_t b) {\n"
        "    return jtoc_i32((int64_t)a - (int64_t)b);\n"
        "}\n"
        "static inline int32_t jtoc_imul(int32_t a, int32_t b) {\n"
        "    return jtoc_i32((int64_t)a * (int64_t)b);\n"
        "}\n"
        "static inline int32_t jtoc_ineg(int32_t a) {\n"
        "    return jtoc_i32(-(int64_t)a);\n"
        "}\n"
        "static inline void jtoc_divisao_por_zero(void) {\n"
        "    fputs(\"Erro de execucao: divisao inteira por zero.\\n\", stderr);\n"
        "    exit(EXIT_FAILURE);\n"
        "}\n"
        "static inline int32_t jtoc_idiv(int32_t a, int32_t b) {\n"
        "    if (b == 0) jtoc_divisao_por_zero();\n"
        "    if (a == INT32_MIN && b == -1) return INT32_MIN;\n"
        "    return (int32_t)((int64_t)a / (int64_t)b);\n"
        "}\n"
        "static inline int32_t jtoc_imod(int32_t a, int32_t b) {\n"
        "    if (b == 0) jtoc_divisao_por_zero();\n"
        "    if (a == INT32_MIN && b == -1) return 0;\n"
        "    return (int32_t)((int64_t)a %% (int64_t)b);\n"
        "}\n\n");
}

static int emitir_strings(GeradorC *g) {
    for (size_t i = 0; i < g->quantidade_strings; i++) {
        if (!escrever(g, "static const char jtoc_s%zu[] = \"", i)) return 0;
        for (size_t j = 0; j < g->strings[i].tamanho; j++) {
            /* Tres digitos octais evitam que o byte seguinte prolongue o escape. */
            if (!escrever(g, "\\%03o", (unsigned int)g->strings[i].bytes[j])) return 0;
        }
        if (!escrever(g, "\";\n")) return 0;
    }
    if (g->quantidade_strings != 0 && !escrever(g, "\n")) return 0;
    return 1;
}

static int emitir_expressao(GeradorC *g, const NoAST *no);

static int emitir_literal(GeradorC *g, const NoAST *no) {
    const char *texto = no->texto;

    if (texto[0] == '"') {
        size_t indice;
        if (!indice_string(g, texto, &indice)) return 0;
        return escrever(g, "jtoc_s%zu", indice);
    }

    if (texto[0] == '\'') {
        unsigned char *bytes = NULL;
        size_t tamanho = 0;
        if (!decodificar_literal(texto, &bytes, &tamanho))
            return erro(g, "literal char possui escape invalido.");
        if (tamanho != 1) {
            free(bytes);
            return erro(g, "char com mais de um byte nao e representavel pelo tipo char do alvo C.");
        }
        int ok = escrever(g, "((char)%u)", (unsigned int)bytes[0]);
        free(bytes);
        return ok;
    }

    if (strcmp(texto, "true") == 0) return escrever(g, "true");
    if (strcmp(texto, "false") == 0) return escrever(g, "false");

    if (tipo_eh(no, "float")) {
        /* O sufixo f impede que uma constante da origem entre como double. */
        return escrever(g, "(%sf)", texto);
    }

    if (tipo_eh(no, "int")) {
        char *fim = NULL;
        errno = 0;
        long long valor = strtoll(texto, &fim, 10);
        if (errno != 0 || fim == texto || *fim != '\0'
                || valor < 0 || valor > INT32_MAX)
            return erro(g, "literal int fora da faixa validada pela analise semantica.");
        return escrever(g, "%" PRId32, (int32_t)valor);
    }

    return erro(g, "literal sem tipo reconhecido.");
}

static int emitir_operando_numerico(GeradorC *g, const NoAST *no, int promover_float) {
    int precisa_converter = promover_float && tipo_eh(no, "int");
    if (precisa_converter && !escrever(g, "((float)(")) return 0;
    if (!emitir_expressao(g, no)) return 0;
    if (precisa_converter && !escrever(g, "))")) return 0;
    return 1;
}

static int emitir_binaria_numerica(GeradorC *g, const NoAST *no, const char *operador) {
    const NoAST *a = no->filho;
    const NoAST *b = irmao(a);
    int promover_float;

    if (a == NULL || b == NULL || irmao(b) != NULL)
        return erro(g, "expressao binaria com numero incorreto de operandos.");
    promover_float = tipo_eh(a, "float") || tipo_eh(b, "float");

    if (promover_float && !escrever(g, "(")) return 0;
    if (!emitir_operando_numerico(g, a, promover_float)) return 0;
    if (!escrever(g, " %s ", operador)) return 0;
    if (!emitir_operando_numerico(g, b, promover_float)) return 0;
    if (promover_float && !escrever(g, ")")) return 0;
    return 1;
}

static int emitir_expressao(GeradorC *g, const NoAST *no) {
    const NoAST *a;
    const NoAST *b;

    if (no == NULL || no->tipo == NULL)
        return erro(g, "expressao ausente ou sem anotacao de tipo.");

    if (no->identificador) return nome_variavel(g, no);

    if (no->inteiro_minimo) return escrever(g, "INT32_MIN");

    a = no->filho;
    b = irmao(a);
    if (a == NULL) return emitir_literal(g, no);

    if (eh(no, "Math.sqrt")) {
        if (b != NULL || irmao(b) != NULL)
            return erro(g, "Math.sqrt recebeu estrutura de operandos invalida.");
        return escrever(g, "((float)sqrt((double)(float)(")
            && emitir_expressao(g, a)
            && escrever(g, ")))" );
    }

    if (eh(no, "Math.pow")) {
        if (b == NULL || irmao(b) != NULL)
            return erro(g, "Math.pow recebeu estrutura de operandos invalida.");
        return escrever(g, "((float)pow((double)(float)(")
            && emitir_operando_numerico(g, a, 1)
            && escrever(g, "), (double)(float)(")
            && emitir_operando_numerico(g, b, 1)
            && escrever(g, ")))" );
    }

    if (eh(no, "!")) {
        if (b != NULL) return erro(g, "negacao logica com mais de um operando.");
        return escrever(g, "(!(") && emitir_expressao(g, a) && escrever(g, "))");
    }

    if (eh(no, "-") && b == NULL) {
        if (tipo_eh(no, "int"))
            return escrever(g, "jtoc_ineg((int32_t)(")
                && emitir_expressao(g, a) && escrever(g, "))");
        return escrever(g, "(-(") && emitir_expressao(g, a) && escrever(g, "))");
    }

    if (eh(no, "&&") || eh(no, "||")) {
        if (b == NULL || irmao(b) != NULL)
            return erro(g, "expressao logica binaria com operandos invalidos.");
        /* Os operadores nativos de C preservam o curto-circuito. */
        return escrever(g, "((") && emitir_expressao(g, a)
            && escrever(g, ") %s (", no->texto)
            && emitir_expressao(g, b) && escrever(g, "))");
    }

    if (eh(no, "+") || eh(no, "-") || eh(no, "*")
            || eh(no, "/") || eh(no, "%")) {
        if (b == NULL || irmao(b) != NULL)
            return erro(g, "operacao aritmetica binaria com operandos invalidos.");

        if (tipo_eh(no, "int")) {
            const char *funcao = eh(no, "+") ? "jtoc_iadd"
                : eh(no, "-") ? "jtoc_isub"
                : eh(no, "*") ? "jtoc_imul"
                : eh(no, "/") ? "jtoc_idiv" : "jtoc_imod";
            return escrever(g, "%s((int32_t)(", funcao)
                && emitir_expressao(g, a)
                && escrever(g, "), (int32_t)(")
                && emitir_expressao(g, b)
                && escrever(g, "))");
        }

        if (eh(no, "%")) {
            return escrever(g, "fmodf((float)(")
                && emitir_operando_numerico(g, a, 1)
                && escrever(g, "), (float)(")
                && emitir_operando_numerico(g, b, 1)
                && escrever(g, "))");
        }

        return emitir_binaria_numerica(g, no, no->texto);
    }

    if (eh(no, "<") || eh(no, "<=") || eh(no, ">") || eh(no, ">=")) {
        if (b == NULL || irmao(b) != NULL)
            return erro(g, "comparacao relacional com operandos invalidos.");
        return escrever(g, "(")
            && emitir_binaria_numerica(g, no, no->texto)
            && escrever(g, ")");
    }

    if (eh(no, "==") || eh(no, "!=")) {
        if (b == NULL || irmao(b) != NULL)
            return erro(g, "comparacao de igualdade com operandos invalidos.");

        if (tipo_eh(a, "String")) {
            /* A IR define igualdade de String por identidade de referencia. */
            return escrever(g, "((") && emitir_expressao(g, a)
                && escrever(g, ") %s (", no->texto)
                && emitir_expressao(g, b) && escrever(g, "))");
        }
        if (tipo_eh(a, "int") || tipo_eh(a, "float")) {
            return escrever(g, "(")
                && emitir_binaria_numerica(g, no, no->texto)
                && escrever(g, ")");
        }
        return escrever(g, "((") && emitir_expressao(g, a)
            && escrever(g, ") %s (", no->texto)
            && emitir_expressao(g, b) && escrever(g, "))");
    }

    return erro(g, "operador ou construcao de expressao nao suportada.");
}

static int emitir_bloco_conteudo(GeradorC *g, const NoAST *bloco);
static int emitir_comando(GeradorC *g, const NoAST *no);

static int abrir_bloco(GeradorC *g, const NoAST *bloco) {
    if (!eh(bloco, "Bloco")) return erro(g, "corpo de comando nao esta representado por Bloco.");
    if (!escrever(g, "{\n")) return 0;
    g->indentacao++;
    if (!emitir_bloco_conteudo(g, bloco)) return 0;
    g->indentacao--;
    if (!indent(g) || !escrever(g, "}\n")) return 0;
    return 1;
}

static int emitir_declaracao(GeradorC *g, const NoAST *no, int ponto_virgula) {
    const NoAST *tipo = no->filho;
    const NoAST *nome = irmao(tipo);
    const NoAST *inicializador = irmao(nome);
    const char *tipo_destino;

    if (tipo == NULL || nome == NULL || !nome->identificador)
        return erro(g, "declaracao possui estrutura invalida.");
    tipo_destino = tipo_c(tipo->texto);
    if (tipo_destino == NULL) return erro(g, "tipo de declaracao nao suportado.");

    if (!escrever(g, "%s ", tipo_destino) || !nome_variavel(g, nome)) return 0;
    if (inicializador != NULL) {
        if (!escrever(g, " = ")) return 0;
        if (tipo_eh(nome, "float") && tipo_eh(inicializador, "int")) {
            if (!escrever(g, "((float)(") || !emitir_expressao(g, inicializador)
                    || !escrever(g, "))")) return 0;
        } else if (!emitir_expressao(g, inicializador)) {
            return 0;
        }
    }
    return !ponto_virgula || escrever(g, ";");
}

static int emitir_atribuicao(GeradorC *g, const NoAST *no, int ponto_virgula) {
    const NoAST *nome = no->filho;
    const NoAST *valor = irmao(nome);
    if (nome == NULL || !nome->identificador || valor == NULL || irmao(valor) != NULL)
        return erro(g, "atribuicao possui estrutura invalida.");

    if (eh(no, "=")) {
        if (!nome_variavel(g, nome) || !escrever(g, " = ")) return 0;
        if (tipo_eh(nome, "float") && tipo_eh(valor, "int")) {
            if (!escrever(g, "((float)(") || !emitir_expressao(g, valor)
                    || !escrever(g, "))")) return 0;
        } else if (!emitir_expressao(g, valor)) {
            return 0;
        }
    } else {
        const char *funcao = NULL;
        if (eh(no, "+=")) funcao = "jtoc_iadd";
        else if (eh(no, "-=")) funcao = "jtoc_isub";
        else if (eh(no, "*=")) funcao = "jtoc_imul";
        else if (eh(no, "/=")) funcao = "jtoc_idiv";
        else return erro(g, "operador de atribuicao composta nao suportado.");

        if (!nome_variavel(g, nome) || !escrever(g, " = ")) return 0;
        if (tipo_eh(nome, "int")) {
            if (!escrever(g, "%s((int32_t)(", funcao) || !nome_variavel(g, nome)
                    || !escrever(g, "), (int32_t)(") || !emitir_expressao(g, valor)
                    || !escrever(g, "))")) return 0;
        } else {
            const char *operador = eh(no, "+=") ? "+"
                : eh(no, "-=") ? "-" : eh(no, "*=") ? "*" : "/";
            if (!escrever(g, "((float)(") || !nome_variavel(g, nome)
                    || !escrever(g, ") %s ", operador)) return 0;
            if (tipo_eh(valor, "int")) {
                if (!escrever(g, "((float)(") || !emitir_expressao(g, valor)
                        || !escrever(g, "))")) return 0;
            } else if (!emitir_expressao(g, valor)) {
                return 0;
            }
            if (!escrever(g, ")")) return 0;
        }
    }

    return !ponto_virgula || escrever(g, ";");
}

static int emitir_incremento(GeradorC *g, const NoAST *no, int ponto_virgula) {
    const NoAST *nome = no->filho;
    if (nome == NULL || !nome->identificador || irmao(nome) != NULL)
        return erro(g, "incremento/decremento possui estrutura invalida.");

    if (!nome_variavel(g, nome) || !escrever(g, " = ")) return 0;
    if (tipo_eh(nome, "int")) {
        if (!escrever(g, "%s((int32_t)(", eh(no, "++") ? "jtoc_iadd" : "jtoc_isub")
                || !nome_variavel(g, nome) || !escrever(g, "), INT32_C(1))")) return 0;
    } else {
        if (!escrever(g, "((float)(") || !nome_variavel(g, nome)
                || !escrever(g, " %s 1.0f))", eh(no, "++") ? "+" : "-")) return 0;
    }
    return !ponto_virgula || escrever(g, ";");
}

static int emitir_clausula_for(GeradorC *g, const NoAST *no) {
    if (no == NULL || eh(no, "Vazio")) return 1;
    if (eh(no, "Declaracao")) return emitir_declaracao(g, no, 0);
    if (eh(no, "=")) return emitir_atribuicao(g, no, 0);
    if (eh(no, "+=") || eh(no, "-=") || eh(no, "*=") || eh(no, "/="))
        return emitir_atribuicao(g, no, 0);
    if (eh(no, "++") || eh(no, "--")) return emitir_incremento(g, no, 0);
    return erro(g, "clausula de for nao suportada.");
}

static int emitir_switch(GeradorC *g, const NoAST *no) {
    const NoAST *seletor = no->filho;
    const NoAST *lista = irmao(seletor);
    if (seletor == NULL || !eh(lista, "Casos") || irmao(lista) != NULL)
        return erro(g, "switch possui estrutura invalida.");

    if (!indent(g) || !escrever(g, "switch (")) return 0;
    if (!emitir_expressao(g, seletor) || !escrever(g, ") {\n")) return 0;
    g->indentacao++;

    for (const NoAST *caso = lista->filho; caso != NULL; caso = caso->irmao) {
        const NoAST *corpo;
        if (eh(caso, "Case")) {
            const NoAST *expressao_caso = caso->filho;
            if (expressao_caso == NULL || !tipo_eh(caso, "int"))
                return erro(g, "case nao possui constante inteira validada.");
            corpo = irmao(expressao_caso);
            if (!eh(corpo, "Bloco") || irmao(corpo) != NULL)
                return erro(g, "corpo de case possui estrutura invalida.");
            if (!indent(g)) return 0;
            if (caso->valor_caso == INT32_MIN) {
                if (!escrever(g, "case INT32_MIN:\n")) return 0;
            } else if (!escrever(g, "case %" PRId32 ":\n", caso->valor_caso)) {
                return 0;
            }
        } else if (eh(caso, "Default")) {
            corpo = caso->filho;
            if (!eh(corpo, "Bloco") || irmao(corpo) != NULL)
                return erro(g, "corpo de default possui estrutura invalida.");
            if (!indent(g) || !escrever(g, "default:\n")) return 0;
        } else {
            return erro(g, "rotulo de switch desconhecido.");
        }

        /* O rotulo precisa preceder uma instrucao; o ponto-e-virgula tambem
           permite declarar variaveis imediatamente depois do rotulo em C11. */
        g->indentacao++;
        if (!indent(g) || !escrever(g, ";\n")) return 0;
        if (!emitir_bloco_conteudo(g, corpo)) return 0;
        g->indentacao--;
        if (caso->irmao != NULL) {
            /* Comentario reconhecido pelo GCC; nao altera o fall-through. */
            if (!indent(g) || !escrever(g, "/* fall through */\n")) return 0;
        }
    }

    g->indentacao--;
    return indent(g) && escrever(g, "}\n");
}

static int emitir_comando(GeradorC *g, const NoAST *no) {
    const NoAST *a;
    if (no == NULL) return erro(g, "comando nulo na AST.");
    a = no->filho;

    if (eh(no, "Bloco")) {
        if (!indent(g)) return 0;
        return abrir_bloco(g, no);
    }

    if (eh(no, "Declaracao")) {
        if (!indent(g) || !emitir_declaracao(g, no, 1)) return 0;
        return escrever(g, "\n");
    }

    if (eh(no, "=")) {
        if (!indent(g) || !emitir_atribuicao(g, no, 1)) return 0;
        return escrever(g, "\n");
    }

    if (eh(no, "+=") || eh(no, "-=") || eh(no, "*=") || eh(no, "/=")) {
        if (!indent(g) || !emitir_atribuicao(g, no, 1)) return 0;
        return escrever(g, "\n");
    }

    if (eh(no, "++") || eh(no, "--")) {
        if (!indent(g) || !emitir_incremento(g, no, 1)) return 0;
        return escrever(g, "\n");
    }

    if (eh(no, "If")) {
        const NoAST *verdadeiro = irmao(a);
        const NoAST *falso = irmao(verdadeiro);
        if (a == NULL || !eh(verdadeiro, "Bloco")
                || (falso != NULL && (!eh(falso, "Bloco") || irmao(falso) != NULL)))
            return erro(g, "if possui estrutura invalida.");
        if (!indent(g) || !escrever(g, "if (")) return 0;
        if (!emitir_expressao(g, a) || !escrever(g, ") ")) return 0;
        if (!abrir_bloco(g, verdadeiro)) return 0;
        if (falso != NULL) {
            if (!indent(g) || !escrever(g, "else ")) return 0;
            if (!abrir_bloco(g, falso)) return 0;
        }
        return 1;
    }

    if (eh(no, "While")) {
        const NoAST *corpo = irmao(a);
        if (a == NULL || !eh(corpo, "Bloco") || irmao(corpo) != NULL)
            return erro(g, "while possui estrutura invalida.");
        if (!indent(g) || !escrever(g, "while (")) return 0;
        if (!emitir_expressao(g, a) || !escrever(g, ") ")) return 0;
        return abrir_bloco(g, corpo);
    }

    if (eh(no, "For")) {
        const NoAST *condicao;
        const NoAST *atualizacao;
        const NoAST *corpo;
        if (a == NULL || (condicao = irmao(a)) == NULL
                || (atualizacao = irmao(condicao)) == NULL
                || (corpo = irmao(atualizacao)) == NULL || irmao(corpo) != NULL
                || !eh(corpo, "Bloco"))
            return erro(g, "for deve conter inicializacao, condicao, atualizacao e corpo.");
        if (!indent(g) || !escrever(g, "for (")) return 0;
        if (!emitir_clausula_for(g, a) || !escrever(g, "; ")) return 0;
        if (!eh(condicao, "Vazio") && !emitir_expressao(g, condicao)) return 0;
        if (!escrever(g, "; ") || !emitir_clausula_for(g, atualizacao)
                || !escrever(g, ") ")) return 0;
        return abrir_bloco(g, corpo);
    }

    if (eh(no, "Switch")) return emitir_switch(g, no);

    if (eh(no, "Break")) {
        if (a != NULL) return erro(g, "break possui filhos inesperados.");
        return indent(g) && escrever(g, "break;\n");
    }

    if (eh(no, "Print") || eh(no, "Println")) {
        if (a == NULL || irmao(a) != NULL) return erro(g, "print possui estrutura invalida.");
        if (!indent(g)) return 0;
        if (tipo_eh(a, "int")) {
            if (!escrever(g, "printf(\"%%d\", ")) return 0;
            if (!emitir_expressao(g, a)) return 0;
            return escrever(g, ");\n") && (!eh(no, "Println") || (indent(g) && escrever(g, "printf(\"\\n\");\n")));
        }
        if (tipo_eh(a, "float")) {
            if (!escrever(g, "printf(\"%%f\", (double)(") || !emitir_expressao(g, a)
                    || !escrever(g, "));\n")) return 0;
            return !eh(no, "Println") || (indent(g) && escrever(g, "printf(\"\\n\");\n"));
        }
        if (tipo_eh(a, "char")) {
            if (!escrever(g, "printf(\"%%c\", (int)(unsigned char)(") || !emitir_expressao(g, a)
                    || !escrever(g, "));\n")) return 0;
            return !eh(no, "Println") || (indent(g) && escrever(g, "printf(\"\\n\");\n"));
        }
        if (tipo_eh(a, "String")) {
            if (!escrever(g, "printf(\"%%s\", ") || !emitir_expressao(g, a)
                    || !escrever(g, ");\n")) return 0;
            return !eh(no, "Println") || (indent(g) && escrever(g, "printf(\"\\n\");\n"));
        }
        if (tipo_eh(a, "boolean")) {
            if (!escrever(g, "printf(\"%%s\", (") || !emitir_expressao(g, a)
                    || !escrever(g, ") ? \"true\" : \"false\");\n")) return 0;
            return !eh(no, "Println") || (indent(g) && escrever(g, "printf(\"\\n\");\n"));
        }
        return erro(g, "tipo de expressao nao suportado por print.");
    }

    if (eh(no, "Vazio")) return 1;
    return erro(g, "comando ou identificador estrutural nao suportado.");
}

static int emitir_bloco_conteudo(GeradorC *g, const NoAST *bloco) {
    if (!eh(bloco, "Bloco")) return erro(g, "era esperado um no Bloco.");
    for (const NoAST *comando = bloco->filho; comando != NULL; comando = comando->irmao) {
        if (!emitir_comando(g, comando)) return 0;
    }
    return 1;
}

static int emitir_programa(GeradorC *g, const NoAST *raiz) {
    if (!eh(raiz, "Bloco") || irmao(raiz) != NULL)
        return erro(g, "a raiz da AST nao e um unico Bloco.");

    if (!emitir_cabecalho(g) || !emitir_auxiliares(g) || !emitir_strings(g)) return 0;
    if (!escrever(g, "int main(void) {\n")) return 0;
    g->indentacao = 1;
    if (!emitir_bloco_conteudo(g, raiz)) return 0;
    g->indentacao = 0;
    return escrever(g, "    return 0;\n}\n");
}

static void liberar_recursos(GeradorC *g) {
    for (size_t i = 0; i < g->quantidade_strings; i++) free(g->strings[i].bytes);
    free(g->strings);
}

int gerar_codigo_c(const NoAST *raiz, FILE *saida) {
    GeradorC g = {0};
    FILE *temporario;
    int sucesso = 0;

    if (raiz == NULL || saida == NULL) {
        fprintf(stderr, "Geracao C: raiz da AST e arquivo de saida sao obrigatorios.\n");
        return 0;
    }

    temporario = tmpfile();
    if (temporario == NULL) {
        fprintf(stderr, "Geracao C: nao foi possivel criar arquivo temporario.\n");
        return 0;
    }
    g.saida = temporario;

    if (coletar_recursos(&g, raiz)
            && emitir_programa(&g, raiz)
            && fflush(temporario) == 0
            && fseek(temporario, 0, SEEK_SET) == 0) {
        unsigned char buffer[8192];
        size_t lidos;
        sucesso = 1;
        while ((lidos = fread(buffer, 1, sizeof buffer, temporario)) > 0) {
            if (fwrite(buffer, 1, lidos, saida) != lidos) {
                sucesso = 0;
                break;
            }
        }
        if (ferror(temporario) || fflush(saida) != 0) sucesso = 0;
    }

    if (!sucesso && !g.falha)
        fprintf(stderr, "Geracao C: falha de leitura ou escrita do arquivo temporario.\n");
    if (fclose(temporario) != 0) sucesso = 0;
    liberar_recursos(&g);
    return sucesso;
}
