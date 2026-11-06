#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include "ast.h"

/* Uma entrada para cada declaracao encontrada no programa. */
typedef struct Simbolo {
    char *simbolo;
    char *tipo;
    char *valor;
    const char *escopo;
    int id_escopo;
    struct Simbolo *proximo;
} Simbolo;

/* Os pais ficam na pilha durante a visita da AST; a tabela guarda apenas o id. */
typedef struct Escopo {
    int id;
    struct Escopo *pai;
} Escopo;

Simbolo *buscar_no_escopo(const char *nome, int id_escopo);
Simbolo *buscar_simbolo(const char *nome, Escopo *escopo);
void inserir_simbolo(NoAST *declaracao, Escopo *escopo);
void verificar_expressao(NoAST *no, Escopo *escopo);
void analisar_comando(NoAST *comando, Escopo *escopo);
void analisar_comandos(NoAST *bloco, Escopo *escopo);
void montar_tabela(NoAST *bloco, Escopo *pai);
void imprimir_tabela(void);
void liberar_tabela(void);
/* Contador consultado pelo programa apos montar a tabela e validar a AST. */
extern int erros_semanticos;

#endif
