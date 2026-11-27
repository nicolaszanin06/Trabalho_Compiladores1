# Geração final de código C

O modo `--c` traduz a AST anotada pela análise semântica para um programa C11
completo. A geração não consome a IR textual e não executa uma fase de
otimização.

## Uso

```bash
# Gerar C em stdout
./src/jtoc --c programa.java

# Gerar C em um arquivo novo (o destino não pode existir)
./src/jtoc --c -o programa.c programa.java

# -o, sozinho, também seleciona a geração C
./src/jtoc -o programa.c programa.java

# A depuração fica em stderr; stdout continua sendo C compilável
./src/jtoc --c --debug programa.java
```

O modo padrão e `--ir` mantêm suas finalidades anteriores. `--ir` e `--c` são
mutuamente exclusivos. O comando `--ir -o ...` é recusado.

## Compilar o C gerado

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic programa.c -o programa
```

Acrescente `-lm` quando o programa-fonte usar `Math.sqrt`, `Math.pow` ou resto
(`%`) entre valores `float`. Resultados não finitos e erros de domínio seguem o
comportamento da biblioteca matemática e da implementação C; isso não se
confunde com divisão inteira por zero, que recebe diagnóstico próprio:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic programa.c -o programa -lm
```

## Decisões de tradução

- `int` exige que o alvo C represente inteiros com sinal de 32 bits. A geração
  inclui `_Static_assert` para verificar essa condição.
- Aritmética inteira usa auxiliares baseados em `int64_t` e conversão explícita
  para 32 bits; divisão por zero termina com diagnóstico em `stderr` e código
  de saída diferente de zero. Os casos `INT32_MIN / -1` e `INT32_MIN % -1` são
  tratados explicitamente.
- `float` é emitido com sufixo `f`; `Math.sqrt` e `Math.pow` promovem argumentos
  a `float` antes de chamar as funções de `<math.h>`. A impressão de `float`
  usa `%f`, com seis casas decimais.
- `String` é `const char *`. Literais são decodificados e agrupados pelo
  conteúdo, de modo que igualdade (`==`/`!=`) compara referências e literais
  iguais compartilham um único objeto estático. Strings são impressas com
  `printf("%s", ...)`, nunca como formato.
- Variáveis são nomeadas `jtoc_v<ID>` a partir de `id_simbolo`, preservando
  declarações homônimas em escopos diferentes e evitando palavras reservadas e
  nomes de funções da biblioteca C.
- Blocos, condicionais, laços e `switch` são emitidos como construções nativas
  de C. `switch` preserva a ordem dos rótulos e o *fall-through*; nenhum `break`
  é inserido automaticamente.
- `char` é representado como `char` e, por isso, o backend exige que o literal
  tenha um único byte após a decodificação. Strings preservam os bytes UTF-8.

## Limitações

O backend cobre somente as construções reconhecidas pelo subconjunto Java
Procedural atual. Classes, métodos, parâmetros, chamadas gerais, `return`,
concatenação e leitura de entrada continuam fora da gramática. A impressão de
`float` é deliberadamente uma política simples (`%f` com seis casas), não uma
reimplementação de `Float.toString` do Java.

## Testes

Depois de regenerar Flex/Bison e compilar o compilador, execute:

```bash
python testes/test_gerador_c.py
```

A suíte traduz programas pequenos, compila o C resultante com GCC em C11 e
compara a saída do executável. Os testes usam diretórios temporários e possuem
timeouts separados para compilação e execução.

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação do documento | 27/11/2026 | [Henrique](https://github.com/henryqma) |
