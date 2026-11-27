/* Injeta falhas em cada alocacao de texto_expressao e verifica a limpeza. */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static int restantes;
static int pendentes;

static void *alocar_teste(size_t tamanho) {
    if (restantes-- == 0) return NULL;
    void *p = malloc(tamanho);
    assert(p != NULL);
    pendentes++;
    return p;
}

static void liberar_teste(void *p) {
    if (p != NULL) pendentes--;
    free(p);
}

static char *duplicar_teste(const char *texto) {
    size_t tamanho = strlen(texto) + 1;
    char *p = alocar_teste(tamanho);
    if (p != NULL) memcpy(p, texto, tamanho);
    return p;
}

/* A inclusao permite testar sem acrescentar ganchos ao codigo de producao. */
#define malloc alocar_teste
#define free liberar_teste
#define strdup duplicar_teste
#include "ast.c"
#undef malloc
#undef free
#undef strdup

static void conferir(NoAST *no, const char *esperado) {
    for (int falha = 0; falha < 20; falha++) {
        restantes = falha;
        char *texto = texto_expressao(no);
        if (texto != NULL) {
            assert(strcmp(texto, esperado) == 0);
            liberar_teste(texto);
            assert(pendentes == 0);
            return;
        }
        assert(pendentes == 0);
    }
    assert(0 && "Nao chegou ao caminho sem falha");
}

int main(void) {
    NoAST direita = {.texto = "3"};
    NoAST esquerda = {.texto = "2", .irmao = &direita};
    NoAST soma = {.texto = "+", .filho = &esquerda};
    NoAST negativo = {.texto = "-", .filho = &soma};
    conferir(NULL, "nao inicializado");
    conferir(&esquerda, "2");
    conferir(&soma, "(2 + 3)");
    conferir(&negativo, "(-(2 + 3))");
    return 0;
}
