#ifndef GERADOR_C_H
#define GERADOR_C_H

#include "ast.h"

/*
 * Gera um programa C11 completo a partir de uma AST já analisada e validada.
 *
 * A função não modifica nem libera a AST. A saída só é copiada para `saida`
 * depois que toda a tradução foi preparada com sucesso.
 *
 * Retorna 1 em caso de sucesso e 0 em caso de erro.
 */
int gerar_codigo_c(const NoAST *raiz, FILE *saida);

#endif
