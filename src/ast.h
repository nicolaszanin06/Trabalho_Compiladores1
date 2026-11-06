#ifndef AST_H
#define AST_H

/* Cada no guarda seu texto, o primeiro filho e o proximo irmao. */
typedef struct NoAST {
    char *texto;
    struct NoAST *filho;
    struct NoAST *irmao;
} NoAST;

/* criar_no copia o texto; criar_folha tambem libera o texto recebido.
 * Os filhos adicionados passam a pertencer a arvore.
 * liberar_ast libera o no, seus filhos e seus irmaos. */
NoAST *criar_no(const char *texto);
NoAST *criar_folha(char *texto);
NoAST *montar_no(const char *texto, NoAST *a, NoAST *b, NoAST *c);
void adicionar_filho(NoAST *pai, NoAST *filho);
void imprimir_ast(NoAST *no, int nivel);
void liberar_ast(NoAST *no);
/* O chamador deve liberar o texto retornado com free. */
char *texto_expressao(NoAST *no);

#endif
