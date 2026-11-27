# Testes do analisador e do backend

## Suíte automatizada (Windows e Linux)

Requisitos: Python 3 e GCC no `PATH`, sem pacotes adicionais de Python.
Na raiz do repositório, execute:

```powershell
python testes/test_analisador.py
python testes/test_gerador_c.py
```

No Linux, os comandos podem ser `python3 testes/test_analisador.py` e
`python3 testes/test_gerador_c.py`.

A suíte do analisador compila o analisador e um observador de tokens
(`lexer_driver.c`) em uma pasta temporária, removida ao terminar. O observador
chama `yylex()` diretamente e verifica nomes dos tokens, lexemas, linhas e o
lexema inteiro armazenado em `yylval.stringValue`, incluindo zeros à esquerda.

Também compila `alocacao_driver.c`, que injeta falhas em cada alocação de
`texto_expressao()` e verifica se os resultados parciais são liberados.
Não é necessário um executável pré-compilado em `src/`.

Os testes existentes cobrem subcasos para os 14 arquivos de entrada e verificam:

- Palavras reservadas, identificadores, operadores, delimitadores e literais.
- Comentários, espaços, CRLF e linhas dos diagnósticos.
- Tokens de `String`, impressão e funções matemáticas.
- Aceitação ou rejeição das entradas e os códigos de saída 0 ou 1.
- Ausência da mensagem de sucesso e de AST quando há erros.
- Precedência e parênteses na AST.
- Atribuições compostas, cláusulas opcionais do `for`, `switch`, casos agrupados,
  `default`, `break` e rejeição de construções malformadas.
- Tipo, inicialização e escopo na tabela de símbolos, incluindo nomes repetidos
  em blocos distintos e preservação do valor inicial após uma atribuição.
- Rejeição de declarações repetidas no mesmo escopo e de usos sem declaração,
  incluindo a ordem das declarações, blocos irmãos, cabeçalho do `for` e `switch`.
- Rejeição de `break` fora de laços e `switch`, inclusive depois do fechamento
  dessas estruturas; aceitação dentro de `while`, `for` e `switch`, com blocos,
  condicionais e estruturas aninhadas, com e sem `--debug`.
- Execução da IR por `executor_ir.py`: precedência, atualizações, conversões,
  laços aninhados, escopos, `break`, `switch` com *fall-through* e matemática.
- Limite de passos para laços infinitos e pilha vazia ao final da execução.
- Rejeição de inteiros fora de 32 bits e emissão direta de `-2147483648`,
  incluindo uso em `case` e overflow de expressões inteiras.
- Inicialização em `if (1 == 1)` e `while (2 > 1)`, condições desconhecidas
  envolvendo variáveis e curto-circuito de comparações constantes.
- Todos os escapes permitidos (`\b`, `\t`, `\n`, `\f`, `\r`, `\"`, `\'`, `\\`)
  e rejeição de escapes inválidos, octais e Unicode neste subconjunto.

Por padrão, a suíte do analisador compila os arquivos C versionados em `src/`,
incluindo a AST, a tabela de símbolos, o gerador intermediário e o gerador C.
Ao alterar `jtoc.l` ou `jtoc.y`, instale Flex e Bison e execute:

```bash
python testes/test_analisador.py --regenerate
```

Essa opção regenera os arquivos em uma pasta temporária, sem alterar `src/`.
O fluxo `.github/workflows/testes.yml` regenera e executa as duas suítes em cada
push e pull request.

## Testes da geração final de C

Depois de compilar o compilador com o módulo `gerador_c.c`, execute:

```bash
python testes/test_gerador_c.py
```

A suíte chama `jtoc --c`, grava o C emitido em um diretório temporário, compila
o programa com GCC em C11 e executa o binário. Ela verifica precedência,
literais decimais, overflow inteiro, os limites `INT32_MIN / -1` e
`INT32_MIN % -1`, curto-circuito, `for`, `switch` com *fall-through*, identidade
de strings, `%` em `float`, funções matemáticas, divisão inteira por zero,
`--debug`, rejeição de leituras não inicializadas e proteção de destinos
existentes. A compilação e a execução têm timeouts separados.

No PowerShell, `testes/executar.ps1` também compila e executa um programa C que
combina um `for`, atualização inteira, impressão e `Math.sqrt`. O programa
fonte gerado é salvo sem BOM UTF-8 em uma pasta temporária, e os artefatos são
removidos ao final.

## Arquivos de entrada

Todos os arquivos de entrada usam `.java` e são classificados pelo resultado esperado no analisador atual.

| Pasta | Resultado esperado |
| --- | --- |
| `validos/` | Sem erros léxicos, sintáticos ou semânticos; mensagem de sucesso; código 0 |
| `invalidos/` | Erro léxico e/ou sintático; sem mensagem de sucesso; código 1 |

## Casos válidos

- `vazio.java`: entrada vazia.
- `declaracoes.java`: declarações de variáveis.
- `atribuicao_na_declaracao.java`: declaração com inicialização.
- `condicao_com_identificador.java`: variável booleana usada na condição de um `if`.
- `ifs_aninhados.java`: blocos condicionais aninhados.
- `comentarios.java`: espaços e comentários ignorados.

## Casos inválidos

| Arquivo | Erro esperado |
| --- | --- |
| `sem_ponto_e_virgula.java` | Sintático: falta `;` |
| `expressao_ausente.java` | Sintático: falta expressão depois de `=` |
| `texto_solto.java` | Sintático: texto `Ola, mundo` não forma um comando |
| `caractere_invalido.java` | Léxico: símbolo `@` |
| `string_nao_encerrada.java` | Léxico: string sem fechamento |
| `comentario_nao_encerrado.java` | Léxico: comentário de bloco sem fechamento |
| `string_multilinha.java` | Léxico, seguido de sintático: quebra de linha dentro da string |
| `char_multilinha.java` | Léxico, seguido de sintático: quebra de linha dentro do caractere |

Uma falha léxica pode gerar também uma mensagem sintática, pois o parser continua recebendo os tokens restantes. Para demonstrar cada categoria isoladamente, use os exemplos abaixo.

Os casos semânticos são entradas definidas na suíte Python, além dos 14 arquivos.
Por exemplo, `int x; int x;` rejeita redeclaração, `x = 1;` rejeita uso sem
declaração e `if (true) { break; }` rejeita `break` fora de laço ou `switch`.
Todos retornam código 1, escrevem o diagnóstico em stderr e deixam stdout vazio,
com ou sem `--debug`.

## Roteiro de apresentação no WSL

A partir da raiz do projeto:

```bash
make -B -C src

# Dois exemplos corretos
./src/jtoc --debug < testes/validos/declaracoes.java
./src/jtoc --debug < testes/validos/ifs_aninhados.java

# Dois erros sintáticos
./src/jtoc < testes/invalidos/sem_ponto_e_virgula.java
./src/jtoc < testes/invalidos/expressao_ausente.java

# Dois erros léxicos
./src/jtoc < testes/invalidos/caractere_invalido.java
./src/jtoc < testes/invalidos/string_nao_encerrada.java

# Gerar C, compilar e executar
./src/jtoc --c -o /tmp/saida.c testes/validos/declaracoes.java
gcc -std=c11 -Wall -Wextra -Wpedantic /tmp/saida.c -o /tmp/saida
/tmp/saida
```

## Verificação automática no Windows

No PowerShell, com GCC disponível no Windows, execute na raiz:

```powershell
.\testes\executar.ps1
```

O script mantém os testes de análise e IR existentes e acrescenta um caso
end-to-end de geração C, compilação com GCC em C11 e execução. Após alterar
`jtoc.l` ou `jtoc.y`, regenere os arquivos com Flex/Bison antes de executá-lo.

“Válido” significa aceito pelas análises léxica, sintática e semântica. As entradas são comandos do subconjunto procedural, sem classe ou método `main`.
