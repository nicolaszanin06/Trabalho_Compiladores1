#include "tabela_simbolos.h"
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

static int tipo_numerico(const char *tipo) {
    return strcmp(tipo, "int") == 0 || strcmp(tipo, "float") == 0;
}

/* continuação da implementação da analise semantica */
static const char *inferir_tipo_expressao(NoAST *no, Escopo *escopo) {
      if (no == NULL) {
        return NULL;
    }

    if (no->filho != NULL) {
        if (!operador_aritmetico(no->texto) && !operador_relacional(no->texto)) {
            return NULL;
        }

        NoAST *esquerda = no->filho;
        NoAST *direita = esquerda->irmao;
        const char *tipo_esquerda = inferir_tipo_expressao(esquerda, escopo);
        const char *tipo_direita = inferir_tipo_expressao(direita, escopo);

        if (tipo_esquerda == NULL || (direita != NULL && tipo_direita == NULL)) {
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

            if (operador_relacional(no->texto)) {
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

void analisar_comando(NoAST *comando, Escopo *escopo) {
    if (strcmp(comando->texto, "Declaracao") == 0) {
    NoAST *tipo = comando->filho;
    NoAST *nome = tipo->irmao;
    NoAST *inicializador = nome->irmao;

    inserir_simbolo(comando, escopo);
    verificar_expressao(inicializador, escopo);
    const char *tipo_inicializador =
        inferir_tipo_expressao(inicializador, escopo);

    if (tipo_inicializador != NULL) {
        int mesmo_tipo = strcmp(tipo->texto, tipo_inicializador) == 0;
        int permite_conversao = strcmp(tipo->texto, "float") == 0
            && strcmp(tipo_inicializador, "int") == 0;

        if (!mesmo_tipo && !permite_conversao) {
            fprintf(stderr,
                    "Erro semântico: inicializador de '%s' tem tipo '%s'; "
                    "esperado '%s'.\n",
                    nome->texto, tipo_inicializador, tipo->texto);
            erros_semanticos++;
        }
    }
}
     else if (strcmp(comando->texto, "Bloco") == 0) {
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
        Escopo escopo_switch = {proximo_escopo++, escopo};
        NoAST *expressao = comando->filho;
        NoAST *caso = expressao->irmao->filho;
        verificar_expressao(expressao, escopo);
        contextos_break++;
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
        contextos_break--;
    } else if (strcmp(comando->texto, "Break") == 0) {
        if (contextos_break == 0) {
            fprintf(stderr, "Erro semântico: 'break' fora de laço ou switch.\n");
            erros_semanticos++;
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
