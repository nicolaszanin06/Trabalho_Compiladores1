#ifndef GERADOR_INTERMEDIARIO_H
#define GERADOR_INTERMEDIARIO_H

#include "ast.h"

/*
 * Valida a AST e imprime o codigo intermediario.
 *
 * Retorna 1 em caso de sucesso.
 * Retorna 0 quando a geracao nao e possivel.
 *
 * A AST nao e modificada pelo gerador.
 */
int imprimir_codigo_intermediario(const NoAST *bloco);

#endif