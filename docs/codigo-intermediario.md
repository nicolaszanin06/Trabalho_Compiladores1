# Código intermediário

`jtoc --ir` gera instruções de pilha a partir da AST anotada pela análise
semântica. O gerador percorre a árvore primeiro para verificar que consegue
emitir todos os comandos. Em seguida, reinicia a numeração de rótulos e escreve
uma instrução por linha em stdout. Com `--ir --debug`, a AST, a tabela de
símbolos e os diagnósticos ficam em stderr.

Compile antes de executar: não há binário pré-compilado versionado. Na raiz,
use `make -B -C src` (GNU Make 4.3 ou superior), seguido de
`./src/jtoc --ir arquivo.java`. Sem `--ir`, o programa analisa a entrada e
imprime a mensagem de sucesso; com a opção, verifica também inicialização
antes da leitura e emite apenas IR em stdout. Nenhum modo executa a entrada
ou produz código C.

| Instrução | Efeito |
| --- | --- |
| `LOAD_CONST lexema` | Empilha um literal. O lexema identifica o tipo e preserva aspas e escapes de caracteres e strings. |
| `LOAD nome`, `STORE nome` | Carrega uma variável ou retira um valor da pilha para armazená-lo. |
| `ADD`, `SUB`, `MUL`, `DIV`, `MOD` | Consomem dois operandos e empilham o resultado. O operando direito está no topo. |
| `NEG`, `NOT`, `I2F` | Substituem o topo pelo negativo, pela negação booleana ou pela conversão de `int` para `float`. |
| `EQ`, `NE`, `LT`, `LE`, `GT`, `GE` | Consomem dois valores, na ordem esquerda e direita, e empilham um `boolean`. |
| `LABEL L<n>`, `JUMP L<n>` | Definem um rótulo ou desviam incondicionalmente para ele. |
| `JUMP_IF_FALSE L<n>` | Retira a condição booleana do topo e desvia se ela for falsa. |
| `PRINT`, `PRINTLN` | Retiram e imprimem o valor de acordo com seu tipo. `PRINTLN` acrescenta uma quebra de linha. Booleanos são impressos como `true` e `false`. |
| `SQRT`, `POW` | Consomem um ou dois valores `float` e empilham um `float`. Em `POW`, o expoente está no topo. |

Cada expressão deixa um valor na pilha. Declarações com inicializador,
atribuições e atualizações armazenam seu resultado. Declarações sem
inicializador deixam o armazenamento sem valor até a primeira escrita.
Variáveis do escopo externo usam seu nome original. As demais usam
`@<escopo>:<nome>`, com um identificador distinto por escopo.

O gerador usa desvios para que `&&` e `||` avaliem o segundo operando
somente quando necessário. O `switch` armazena o seletor uma vez em
`@tmp<n>`, compara com os casos inteiros e desvia ao caso correspondente,
ao `default` ou ao fim. Os corpos seguem a ordem do programa, permitindo
fall-through. `break` desvia para fora do laço ou do `switch` mais próximo.

Operações numéricas mistas promovem `int` a `float`. Atribuições,
inicializadores e atualizações aceitam tipos iguais ou a promoção
de `int` para `float`. `char` é um tipo próprio neste subconjunto.
Em `String`, `EQ` e `NE` representam comparação de referência.
`Math.sqrt` e `Math.pow` usam `float` para argumentos e resultado como
simplificação do subconjunto, cuja gramática aceita `float` e `int`.

## Literais e aritmética inteira

Os inteiros são decimais de 32 bits com sinal, inclusive quando o lexema tem
zeros à esquerda. Literais fora da faixa são erros semânticos em qualquer
modo. `-2147483648` é marcado na AST e emitido diretamente como
`LOAD_CONST -2147483648`, sem carregar antes uma magnitude fora da faixa.

Nas operações inteiras, overflow retorna circularmente aos 32 bits. `DIV`
trunca em direção a zero; `MOD` usa o resto dessa divisão. A divisão de
`-2147483648` por `-1` resulta em `-2147483648`. Divisão ou resto inteiro por
zero não são calculados pela análise constante. Essa análise é conservadora
e não substitui o tratamento de erros pelo futuro backend.

Strings e caracteres preservam as aspas e os escapes `\b`, `\t`, `\n`, `\f`,
`\r`, `\"`, `\'` e `\\`. Outros escapes, inclusive octais e Unicode, são
rejeitados pelo scanner neste subconjunto.

## Validação por execução

`testes/executor_ir.py` executa as instruções em uma pilha para conferir
resultados, conversões explícitas, desvios e equilíbrio da pilha nos testes.
Inclui inteiros de 32 bits e arredondamento de resultados numéricos para
`float` de 32 bits. O limite padrão de 10000 passos interrompe laços infinitos.
Esse executor é auxiliar de testes; não oferece um runtime completo de Java,
nem define tratamento de todos os casos de domínio e valores não finitos de
ponto flutuante para o futuro backend.
