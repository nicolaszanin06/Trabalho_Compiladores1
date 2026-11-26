# Código intermediário

`jtoc --ir` gera instruções de pilha a partir da AST anotada pela análise
semântica. O gerador percorre a árvore primeiro para verificar que consegue
emitir todos os comandos. Em seguida, reinicia a numeração de rótulos e escreve
uma instrução por linha em stdout. Com `--ir --debug`, a AST, a tabela de
símbolos e os diagnósticos ficam em stderr.

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
