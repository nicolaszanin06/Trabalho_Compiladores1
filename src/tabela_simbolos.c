#include "tabela_simbolos.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Simbolo *tabela_simbolos = NULL;
static int proximo_escopo = 0;
static size_t quantidade_simbolos = 0;
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
    novo->id = quantidade_simbolos++;
    nome->id_simbolo = novo->id;
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
        int sqrt = strcmp(no->texto, "Math.sqrt") == 0;
        int pow = strcmp(no->texto, "Math.pow") == 0;

        NoAST *esquerda = no->filho;
        NoAST *direita = esquerda->irmao;
        const char *tipo_esquerda = inferir_tipo_expressao(esquerda, escopo);
        const char *tipo_direita = inferir_tipo_expressao(direita, escopo);

        if (tipo_esquerda == NULL || (direita != NULL && tipo_direita == NULL)) {
            return NULL;
        }

        if (sqrt || pow) {
            if ((pow && direita == NULL) || (sqrt && direita != NULL)
                    || (direita != NULL && direita->irmao != NULL)
                    || !tipo_numerico(tipo_esquerda)
                    || (direita != NULL && !tipo_numerico(tipo_direita))) {
                fprintf(stderr, "Erro semântico: '%s' exige %d argumento(s) numérico(s).\n",
                        no->texto, pow ? 2 : 1);
                erros_semanticos++;
                return NULL;
            }
            /* O subconjunto não oferece double. */
            return "float";
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

        if (relacional) return "boolean";
        if (!operador_aritmetico(no->texto)) return NULL;

        if (strcmp(tipo_esquerda, "float") == 0
                || (direita != NULL && strcmp(tipo_direita, "float") == 0)) {
            return "float";
        }

        return "int";
    }

    if (no->identificador) {
        Simbolo *simbolo = buscar_simbolo(no->texto, escopo);
        if (simbolo == NULL) {
            fprintf(stderr, "Erro semântico: Variável não declarada '%s'.\n", no->texto);
            erros_semanticos++;
            return NULL;
        }
        no->id_escopo = simbolo->id_escopo;
        no->id_simbolo = simbolo->id;
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
    if (!condicao->identificador && strcmp(condicao->texto, "Vazio") == 0) {
        return;
    }


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
    } else if (strcmp(comando->texto, "Print") == 0 || strcmp(comando->texto, "Println") == 0) {
        inferir_tipo_expressao(comando->filho, escopo);
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

/* Estado nos caminhos que realmente chegam a um ponto do comando. */
typedef struct Fluxo {
    unsigned char *inicializados;
    int alcancavel;
} Fluxo;

static Fluxo criar_fluxo(const Fluxo *origem, int alcancavel) {
    Fluxo fluxo = {calloc(quantidade_simbolos ? quantidade_simbolos : 1, 1), alcancavel};
    if (fluxo.inicializados == NULL) {
        fprintf(stderr, "Memoria insuficiente para validar inicializacao.\n");
        erros_semanticos++;
    } else if (origem != NULL) {
        memcpy(fluxo.inicializados, origem->inicializados, quantidade_simbolos);
    }
    return fluxo;
}

static void unir_fluxos(Fluxo *destino, const Fluxo *origem) {
    if (!origem->alcancavel) return;
    if (!destino->alcancavel) {
        memcpy(destino->inicializados, origem->inicializados, quantidade_simbolos);
        destino->alcancavel = 1;
        return;
    }
    /* Uma variável só está inicializada se todos os caminhos a escreveram. */
    for (size_t i = 0; i < quantidade_simbolos; i++)
        destino->inicializados[i] &= origem->inicializados[i];
}

typedef enum Condicao { DESCONHECIDA, FALSA, VERDADEIRA } Condicao;

/* Reconhece apenas condições que podem ser decididas por literais booleanos. */
static Condicao condicao_constante(const NoAST *no) {
    if (no->identificador) return DESCONHECIDA;
    if (strcmp(no->texto, "true") == 0 || strcmp(no->texto, "Vazio") == 0) return VERDADEIRA;
    if (strcmp(no->texto, "false") == 0) return FALSA;
    if (strcmp(no->texto, "!") == 0) {
        Condicao operando = condicao_constante(no->filho);
        if (operando != DESCONHECIDA) return operando == FALSA ? VERDADEIRA : FALSA;
    }
    if (strcmp(no->texto, "&&") == 0 || strcmp(no->texto, "||") == 0) {
        int conjuncao = strcmp(no->texto, "&&") == 0;
        Condicao esquerda = condicao_constante(no->filho);
        if (esquerda == (conjuncao ? FALSA : VERDADEIRA)) return esquerda;
        if (esquerda != DESCONHECIDA) return condicao_constante(no->filho->irmao);
    }
    return DESCONHECIDA;
}

static void verificar_leituras(const NoAST *no, const Fluxo *fluxo) {
    if (no == NULL) return;
    if (no->identificador && !fluxo->inicializados[no->id_simbolo]) {
        fprintf(stderr, "Erro semântico: Variável não inicializada '%s'.\n", no->texto);
        erros_semanticos++;
    }

    const NoAST *a = no->filho;
    verificar_leituras(a, fluxo);
    if (a == NULL || a->irmao == NULL) return;
    if (strcmp(no->texto, "&&") == 0 && condicao_constante(a) == FALSA) return;
    if (strcmp(no->texto, "||") == 0 && condicao_constante(a) == VERDADEIRA) return;
    verificar_leituras(a->irmao, fluxo);
}

static void verificar_fluxo(const NoAST *no, Fluxo *fluxo, Fluxo *saida_break);

static void verificar_fluxo_if(const NoAST *condicao, Fluxo *fluxo, Fluxo *saida_break) {
    const NoAST *verdadeiro = condicao->irmao;
    const NoAST *falso = verdadeiro->irmao;
    verificar_leituras(condicao, fluxo);
    Condicao valor = condicao_constante(condicao);
    if (valor != DESCONHECIDA) {
        verificar_fluxo(valor == VERDADEIRA ? verdadeiro : falso, fluxo, saida_break);
        return;
    }

    Fluxo alternativo = criar_fluxo(fluxo, 1);
    if (alternativo.inicializados == NULL) return;
    verificar_fluxo(verdadeiro, fluxo, saida_break);
    verificar_fluxo(falso, &alternativo, saida_break);
    unir_fluxos(fluxo, &alternativo);
    free(alternativo.inicializados);
}

static void substituir_fluxo(Fluxo *destino, Fluxo origem) {
    free(destino->inicializados);
    *destino = origem;
}

static void verificar_fluxo_laco(const NoAST *no, Fluxo *fluxo) {
    const NoAST *condicao = no->filho;
    const NoAST *atualizacao = NULL;
    if (strcmp(no->texto, "For") == 0) {
        verificar_fluxo(no->filho, fluxo, NULL);
        condicao = no->filho->irmao;
        atualizacao = condicao->irmao;
    }
    verificar_leituras(condicao, fluxo);
    Condicao valor = condicao_constante(condicao);
    if (valor == FALSA) return;

    /* A condição pode falhar na primeira passagem; break também pode sair. */
    Fluxo fim = criar_fluxo(fluxo, valor != VERDADEIRA);
    if (fim.inicializados == NULL) return;
    const NoAST *corpo = atualizacao != NULL ? atualizacao->irmao : condicao->irmao;
    verificar_fluxo(corpo, fluxo, &fim);
    verificar_fluxo(atualizacao, fluxo, NULL);
    substituir_fluxo(fluxo, fim);
}

static void verificar_fluxo_switch(const NoAST *seletor, Fluxo *fluxo) {
    verificar_leituras(seletor, fluxo);
    const NoAST *primeiro = seletor->irmao->filho;
    int tem_default = 0;
    for (const NoAST *caso = primeiro; caso != NULL; caso = caso->irmao) {
        if (strcmp(caso->texto, "Default") == 0) tem_default = 1;
    }

    Fluxo entrada = criar_fluxo(fluxo, 1);
    if (entrada.inicializados == NULL) return;
    Fluxo fim = criar_fluxo(fluxo, !tem_default);
    if (fim.inicializados == NULL) {
        free(entrada.inicializados);
        return;
    }
    for (const NoAST *caso = primeiro; caso != NULL; caso = caso->irmao) {
        /* É sempre possível entrar diretamente pelo rótulo deste caso. */
        unir_fluxos(fluxo, &entrada);
        const NoAST *corpo = strcmp(caso->texto, "Default") == 0
            ? caso->filho : caso->filho->irmao;
        verificar_fluxo(corpo, fluxo, &fim);
    }
    unir_fluxos(&fim, fluxo);
    substituir_fluxo(fluxo, fim);
    free(entrada.inicializados);
}

static void verificar_fluxo(const NoAST *no, Fluxo *fluxo, Fluxo *saida_break) {
    if (no == NULL || !fluxo->alcancavel) return;
    const NoAST *a = no->filho;
    if (strcmp(no->texto, "Bloco") == 0) {
        for (const NoAST *comando = a; comando != NULL; comando = comando->irmao)
            verificar_fluxo(comando, fluxo, saida_break);
    } else if (strcmp(no->texto, "If") == 0) {
        verificar_fluxo_if(a, fluxo, saida_break);
    } else if (strcmp(no->texto, "For") == 0 || strcmp(no->texto, "While") == 0) {
        verificar_fluxo_laco(no, fluxo);
    } else if (strcmp(no->texto, "Switch") == 0) {
        verificar_fluxo_switch(a, fluxo);
    } else if (strcmp(no->texto, "Break") == 0) {
        unir_fluxos(saida_break, fluxo);
        fluxo->alcancavel = 0;
    } else if (strcmp(no->texto, "Declaracao") == 0) {
        const NoAST *nome = a->irmao;
        verificar_leituras(nome->irmao, fluxo);
        fluxo->inicializados[nome->id_simbolo] = nome->irmao != NULL;
    } else if (strcmp(no->texto, "=") == 0 || operador_atualizacao(no->texto)) {
        if (operador_atualizacao(no->texto)) verificar_leituras(a, fluxo);
        verificar_leituras(a->irmao, fluxo);
        fluxo->inicializados[a->id_simbolo] = 1;
    } else {
        verificar_leituras(a, fluxo);
    }
}

void verificar_inicializacao(const NoAST *bloco) {
    Fluxo fluxo = criar_fluxo(NULL, 1);
    if (fluxo.inicializados == NULL) return;
    verificar_fluxo(bloco, &fluxo, NULL);
    free(fluxo.inicializados);
}

void imprimir_tabela(FILE *saida) {
    Simbolo *atual = tabela_simbolos;
    fprintf(saida, "\nTabela de simbolos (valor de inicializacao):\n");
    fprintf(saida, "%-20s | %-10s | %-30s | %s\n", "Simbolo", "Tipo", "Valor", "Escopo");
    while (atual != NULL) {
        fprintf(saida, "%-20s | %-10s | %-30s | %s\n",
               atual->simbolo, atual->tipo, atual->valor, atual->escopo);
        atual = atual->proximo;
    }
}

void liberar_tabela(void) {
    proximo_escopo = 0;
    quantidade_simbolos = 0;
    while (tabela_simbolos != NULL) {
        Simbolo *proximo = tabela_simbolos->proximo;
        free(tabela_simbolos->simbolo);
        free(tabela_simbolos->valor);
        free(tabela_simbolos);
        tabela_simbolos = proximo;
    }
}
