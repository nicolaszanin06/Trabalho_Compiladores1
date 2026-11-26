#ifndef AST_H
#define AST_H

#include <stdint.h>
#include <stdio.h>

/* Cada no guarda seu texto, o primeiro filho e o proximo irmao. */
typedef struct NoAST {
    char *texto;
    int identificador;
    int id_escopo;
    size_t id_simbolo;
    int32_t valor_caso;
    const char *tipo; /* Tipo resolvido pela analise semantica. */
    struct NoAST *filho;
    struct NoAST *irmao;
} NoAST;

/* criar_no copia o texto; criar_folha tambem libera o texto recebido.
 * Os filhos adicionados passam a pertencer a arvore.
 * liberar_ast libera o no, seus filhos e seus irmaos. */
NoAST *criar_no(const char *texto);
NoAST *criar_folha(char *texto);
/* Marca usos e nomes de variaveis, sem confundi-los com literais. */
NoAST *criar_identificador(char *texto);
NoAST *montar_no(const char *texto, NoAST *a, NoAST *b, NoAST *c);
void adicionar_filho(NoAST *pai, NoAST *filho);
void imprimir_ast(const NoAST *no, int nivel, FILE *saida);
void liberar_ast(NoAST *no);
/* O chamador deve liberar o texto retornado com free. */
char *texto_expressao(NoAST *no);

#endif
