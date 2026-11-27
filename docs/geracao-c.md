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

A ordem de `--c` e `-o` é livre: `./src/jtoc -o programa.c --c programa.java`
equivale a `./src/jtoc --c -o programa.c programa.java`. Repetir `--c` ou
`--ir` não muda o modo selecionado. Misturar C e IR, repetir `-o` ou omitir
seu argumento resulta em erro de uso antes de criar qualquer saída.

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
  Referências aos literais usam o endereço explícito `&jtoc_s<ID>[0]`, evitando
  warnings de comparação entre arrays sem mudar a identidade das strings.
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

## Detalhes de emissão e publicação

O gerador coleta literais `String` e a necessidade de `<math.h>`, emite cabeçalhos, auxiliares inteiros, arrays estáticos e `int main(void)`, com retorno zero no fim. A tradução é preparada em arquivo temporário. A CLI só publica o C após sucesso da geração; com `-o`, abre um destino novo em modo `wbx`. Erros de tradução não criam esse destino, e falhas de escrita no arquivo criado tentam removê-lo. Transmissões para stdout não podem ser desfeitas após uma falha de E/S.

`jtoc_iadd`, `jtoc_isub`, `jtoc_imul` e `jtoc_ineg` evitam overflow de inteiro com sinal no C usando cálculo em `int64_t` e retorno explícito aos 32 bits. Essa política também vale para compostas e incremento/decremento. `jtoc_idiv` trunca em direção a zero e `jtoc_imod` preserva o sinal do dividendo no resto; para o mínimo inteiro com divisor `-1`, retornam respectivamente `INT32_MIN` e zero.

Divisão **e resto** inteiro por zero chamam `jtoc_divisao_por_zero`, escrevem `Erro de execucao: divisao inteira por zero.` em stderr e terminam com `EXIT_FAILURE`. Isso ocorre no executável gerado: um programa contendo `1 / 0` pode ser traduzido com sucesso e falhar quando executado. O curto-circuito pode impedir que essa operação seja avaliada.

Resto com `float` usa `fmodf`; literais levam sufixo `f` e inteiros com zeros à esquerda são normalizados em decimal, evitando interpretação octal em C. O mínimo inteiro é emitido como `INT32_MIN`. Os limites de `float` são checados na semântica, inclusive underflow para zero; subnormais representáveis são aceitos.

Após cada rótulo de `switch`, emite-se uma instrução vazia `;`, permitindo declarações imediatamente após o rótulo em C11 sem criar outro escopo. Comentários `/* fall through */` preservam a queda entre casos e orientam os avisos do GCC. Para lógica, os operadores nativos `&&` e `||` preservam curto-circuito. Os demais operandos seguem as regras de avaliação do C; o subconjunto não permite atribuições ou incrementos dentro de expressões.

Os bytes das strings são escritos como escapes octais de três dígitos no **C gerado**; isso não permite escapes octais na entrada Java Procedural. Literais de mesmo conteúdo decodificado compartilham referência, incluindo grafias com escapes equivalentes. `printf` usa `%d`, `%f`, `%c` ou `%s` conforme o tipo; booleanos são `true`/`false` e `println` acrescenta uma quebra de linha.

A geração pode produzir avisos legítimos, como variável não utilizada; os testes exigem ausência de warnings apenas nos casos selecionados. Consulte [Arquitetura](arquitetura-decisoes.md) e [Testes](testes.md).

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação do documento | 27/11/2026 | [Henrique](https://github.com/henryqma) |
