#include "tabela_simbolos.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Simbolo *tabela_simbolos = NULL;
static int proximo_escopo = 0;
/* Quantidade de lacos e switches envolventes durante a visita da AST. */
static int contextos_break = 0;
int erros_semanticos = 0;

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
    novo->tipo = tipo->texto;
    nome->tipo = novo->tipo;
    nome->id_escopo = escopo->id;
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

static int operador_aritmetico(const char *texto) {
    return strcmp(texto, "+") == 0
        || strcmp(texto, "-") == 0
        || strcmp(texto, "*") == 0
        || strcmp(texto, "/") == 0
        || strcmp(texto, "%") == 0;
}

static int operador_relacional(const char *texto) {
    return strcmp(texto, "<") == 0
        || strcmp(texto, "<=") == 0
        || strcmp(texto, ">") == 0
        || strcmp(texto, ">=") == 0;
}

static int operador_logico(const char *texto) {
    return strcmp(texto, "&&") == 0
        || strcmp(texto, "||") == 0
        || strcmp(texto, "!") == 0;
}

static int operador_igualdade(const char *texto) {
    return strcmp(texto, "==") == 0 || strcmp(texto, "!=") == 0;
}

static int tipo_numerico(const char *tipo) {
    return strcmp(tipo, "int") == 0 || strcmp(tipo, "float") == 0;
}

static const char *inferir_tipo_expressao(NoAST *no, Escopo *escopo);

static const char *calcular_tipo(NoAST *no, Escopo *escopo) {
    if (no == NULL) {
        return NULL;
    }

    if (no->filho != NULL) {
        int logico = operador_logico(no->texto);
        int igualdade = operador_igualdade(no->texto);
        int relacional = operador_relacional(no->texto);
        if (!logico && !igualdade && !relacional && !operador_aritmetico(no->texto)) {
            return NULL;
        }

        NoAST *esquerda = no->filho;
        NoAST *direita = esquerda->irmao;
        const char *tipo_esquerda = inferir_tipo_expressao(esquerda, escopo);
        const char *tipo_direita = inferir_tipo_expressao(direita, escopo);

        if (tipo_esquerda == NULL || (direita != NULL && tipo_direita == NULL)) {
            return NULL;
        }

        if (logico) {
            if (strcmp(tipo_esquerda, "boolean") == 0
                    && (direita == NULL || strcmp(tipo_direita, "boolean") == 0)) {
                return "boolean";
            }
            fprintf(stderr,
                    "Erro semântico: operador '%s' exige operandos booleanos.\n",
                    no->texto);
            erros_semanticos++;
            return NULL;
        }

        if (igualdade) {
            if (strcmp(tipo_esquerda, tipo_direita) == 0
                    || (tipo_numerico(tipo_esquerda) && tipo_numerico(tipo_direita))) {
                return "boolean";
            }
            fprintf(stderr,
                    "Erro semântico: operador '%s' recebeu tipos incompatíveis '%s' e '%s'.\n",
                    no->texto, tipo_esquerda, tipo_direita);
            erros_semanticos++;
            return NULL;
        }

        if (!tipo_numerico(tipo_esquerda)
                || (direita != NULL && !tipo_numerico(tipo_direita))) {
            fprintf(stderr,
                    "Erro semântico: operador '%s' exige operandos numéricos.\n",
                    no->texto);
            erros_semanticos++;
            return NULL;
        }

        if (relacional) {
            return "boolean";
        }

        if (strcmp(tipo_esquerda, "float") == 0
                || (direita != NULL && strcmp(tipo_direita, "float") == 0)) {
            return "float";
        }

        return "int";
    }

    if (no->identificador) {
        Simbolo *simbolo = buscar_simbolo(no->texto, escopo);
        if (simbolo == NULL) {
            return NULL;
        }
        no->id_escopo = simbolo->id_escopo;
        return simbolo->tipo;
    }

    const char *texto = no->texto;

    if (strcmp(texto, "true") == 0 || strcmp(texto, "false") == 0) {
        return "boolean";
    }
    if (texto[0] == '\'') {
        return "char";
    }
    if (texto[0] == '"') {
        return "String";
    }
    if (texto[0] >= '0' && texto[0] <= '9') {
        if (strchr(texto, '.') != NULL) {
            return "float";
        }
        return "int";
    }

    return NULL;
}

static const char *inferir_tipo_expressao(NoAST *no, Escopo *escopo) {
    if (no == NULL) return NULL;
    no->tipo = calcular_tipo(no, escopo);
    return no->tipo;
}

static void verificar_condicao(NoAST *condicao, Escopo *escopo) {
    if (strcmp(condicao->texto, "Vazio") == 0) {
        return;
    }

    verificar_expressao(condicao, escopo);

    const char *tipo = inferir_tipo_expressao(condicao, escopo);

    if (tipo != NULL && strcmp(tipo, "boolean") != 0) {
        fprintf(stderr,
                "Erro semântico: condição deve ser boolean, recebeu '%s'.\n",
                tipo);
        erros_semanticos++;
    }
}

static int tipos_compativeis(const char *destino, const char *origem) {
    return strcmp(destino, origem) == 0
        || (strcmp(destino, "float") == 0 && strcmp(origem, "int") == 0);
}

static void verificar_atribuicao(NoAST *comando, Escopo *escopo) {
    NoAST *nome = comando->filho;
    NoAST *expressao = nome->irmao;

    verificar_expressao(comando, escopo);

    inferir_tipo_expressao(nome, escopo);
    Simbolo *destino = buscar_simbolo(nome->texto, escopo);
    const char *origem = inferir_tipo_expressao(expressao, escopo);
    if (destino != NULL && origem != NULL && !tipos_compativeis(destino->tipo, origem)) {
        fprintf(stderr,
                "Erro semântico: atribuição a '%s' tem tipo '%s'; esperado '%s'.\n",
                nome->texto, origem, destino->tipo);
        erros_semanticos++;
    }
}

static int operador_atualizacao(const char *texto) {
    return strcmp(texto, "+=") == 0
        || strcmp(texto, "-=") == 0
        || strcmp(texto, "*=") == 0
        || strcmp(texto, "/=") == 0
        || strcmp(texto, "++") == 0
        || strcmp(texto, "--") == 0;
}

static void verificar_atualizacao(NoAST *comando, Escopo *escopo) {
    NoAST *nome = comando->filho;
    NoAST *expressao = nome->irmao;

    verificar_expressao(comando, escopo);

    inferir_tipo_expressao(nome, escopo);
    Simbolo *destino = buscar_simbolo(nome->texto, escopo);
    const char *origem = inferir_tipo_expressao(expressao, escopo);
    if (destino == NULL || (expressao != NULL && origem == NULL)) {
        return;
    }

    if (!tipo_numerico(destino->tipo) || (origem != NULL && !tipo_numerico(origem))) {
        fprintf(stderr,
                "Erro semântico: operador '%s' exige operandos numéricos.\n",
                comando->texto);
        erros_semanticos++;
    } else if (origem != NULL && !tipos_compativeis(destino->tipo, origem)) {
        fprintf(stderr, "Erro semântico: atualização de '%s' tem tipo incompatível '%s'.\n",
                nome->texto, origem);
        erros_semanticos++;
    }
}

/* Expressões constantes int seguem a aritmética de 32 bits. */
static int32_t inteiro32(int64_t valor) {
    uint32_t bits = (uint32_t)valor;
    return bits <= INT32_MAX ? (int32_t)bits : (int32_t)((int64_t)bits - INT64_C(4294967296));
}

static int constante_inteira(const NoAST *no, int32_t *resultado) {
    if (no->tipo == NULL || strcmp(no->tipo, "int") != 0 || no->identificador) return 0;
    const NoAST *a = no->filho;
    if (a == NULL) {
        errno = 0;
        char *fim;
        long long valor = strtoll(no->texto, &fim, 10);
        if (errno != 0 || *fim != '\0' || valor > INT32_MAX) return 0;
        *resultado = (int32_t)valor;
        return 1;
    }

    int32_t esquerda, direita;
    const NoAST *b = a->irmao;
    if (b == NULL && strcmp(no->texto, "-") == 0 && a->filho == NULL
            && !a->identificador) {
        errno = 0;
        char *fim;
        long long valor = strtoll(a->texto, &fim, 10);
        if (errno != 0 || *fim != '\0' || valor > -(int64_t)INT32_MIN) return 0;
        *resultado = (int32_t)-valor;
        return 1;
    }
    if (!constante_inteira(a, &esquerda)) return 0;
    if (b == NULL) {
        if (strcmp(no->texto, "-") != 0) return 0;
        *resultado = inteiro32(-(int64_t)esquerda);
        return 1;
    }
    if (!constante_inteira(b, &direita)) return 0;

    switch (no->texto[0]) {
        case '+': *resultado = inteiro32((int64_t)esquerda + direita); break;
        case '-': *resultado = inteiro32((int64_t)esquerda - direita); break;
        case '*': *resultado = inteiro32((int64_t)esquerda * direita); break;
        case '/':
            if (direita == 0) return 0;
            *resultado = inteiro32((int64_t)esquerda / direita);
            break;
        case '%':
            if (direita == 0) return 0;
            *resultado = (int32_t)((int64_t)esquerda % direita);
            break;
        default: return 0;
    }
    return 1;
}

static void analisar_switch(NoAST *comando, Escopo *escopo) {
    Escopo escopo_switch = {proximo_escopo++, escopo};
    NoAST *seletor = comando->filho;
    NoAST *primeiro = seletor->irmao->filho;
    verificar_expressao(seletor, escopo);
    const char *tipo = inferir_tipo_expressao(seletor, escopo);
    if (tipo != NULL && strcmp(tipo, "int") != 0) {
        fprintf(stderr, "Erro semântico: seletor de switch deve ser int.\n");
        erros_semanticos++;
    }

    int tem_default = 0;
    contextos_break++;
    for (NoAST *caso = primeiro; caso != NULL; caso = caso->irmao) {
        NoAST *corpo = caso->filho;
        if (strcmp(caso->texto, "Case") == 0) {
            verificar_expressao(corpo, &escopo_switch);
            inferir_tipo_expressao(corpo, &escopo_switch);
            caso->tipo = NULL;
            if (!constante_inteira(corpo, &caso->valor_caso)) {
                fprintf(stderr, "Erro semântico: case deve ser uma expressão constante int.\n");
                erros_semanticos++;
            } else {
                caso->tipo = "int";
                for (NoAST *anterior = primeiro; anterior != caso; anterior = anterior->irmao) {
                    if (anterior->tipo != NULL && anterior->valor_caso == caso->valor_caso) {
                        fprintf(stderr, "Erro semântico: valor de case duplicado.\n");
                        erros_semanticos++;
                        break;
                    }
                }
            }
            corpo = corpo->irmao;
        } else if (tem_default++) {
            fprintf(stderr, "Erro semântico: default duplicado.\n");
            erros_semanticos++;
        }
        /* Case nao abre escopo; apenas chaves explicitas abrem outro. */
        analisar_comandos(corpo, &escopo_switch);
    }
    contextos_break--;
}

void analisar_comando(NoAST *comando, Escopo *escopo) {
    if (strcmp(comando->texto, "Declaracao") == 0) {
        NoAST *tipo = comando->filho;
        NoAST *nome = tipo->irmao;
        NoAST *inicializador = nome->irmao;

        inserir_simbolo(comando, escopo);
        verificar_expressao(inicializador, escopo);
        const char *tipo_inicializador = inferir_tipo_expressao(inicializador, escopo);

        if (tipo_inicializador != NULL && !tipos_compativeis(tipo->texto, tipo_inicializador)) {
            fprintf(stderr,
                    "Erro semântico: inicializador de '%s' tem tipo '%s'; "
                    "esperado '%s'.\n",
                    nome->texto, tipo_inicializador, tipo->texto);
            erros_semanticos++;
        }
    } else if (strcmp(comando->texto, "=") == 0) {
        verificar_atribuicao(comando, escopo);
    } else if (operador_atualizacao(comando->texto)) {
        verificar_atualizacao(comando, escopo);
    } else if (strcmp(comando->texto, "Bloco") == 0) {
        montar_tabela(comando, escopo);
    } else if (strcmp(comando->texto, "If") == 0 ||
               strcmp(comando->texto, "While") == 0) {
        NoAST *condicao = comando->filho;
        NoAST *corpo = condicao->irmao;
        int laco = strcmp(comando->texto, "While") == 0;
        verificar_condicao(condicao, escopo);
        if (laco) contextos_break++;
        while (corpo != NULL) {
            montar_tabela(corpo, escopo);
            corpo = corpo->irmao;
        }
        if (laco) contextos_break--;
    } else if (strcmp(comando->texto, "For") == 0) {
        Escopo escopo_for = {proximo_escopo++, escopo};
        NoAST *inicializacao = comando->filho;
        NoAST *condicao = inicializacao->irmao;
        NoAST *atualizacao = condicao->irmao;
        NoAST *corpo = atualizacao->irmao;
        analisar_comando(inicializacao, &escopo_for);
        verificar_condicao(condicao, &escopo_for);
        analisar_comando(atualizacao, &escopo_for);
        contextos_break++;
        montar_tabela(corpo, &escopo_for);
        contextos_break--;
    } else if (strcmp(comando->texto, "Switch") == 0) {
        analisar_switch(comando, escopo);
    } else if (strcmp(comando->texto, "Break") == 0) {
        if (contextos_break == 0) {
            fprintf(stderr, "Erro semântico: 'break' fora de laço ou switch.\n");
            erros_semanticos++;
        }
    } else {
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
    proximo_escopo = 0;
    while (tabela_simbolos != NULL) {
        Simbolo *proximo = tabela_simbolos->proximo;
        free(tabela_simbolos->simbolo);
        free(tabela_simbolos->valor);
        free(tabela_simbolos);
        tabela_simbolos = proximo;
    }
}
