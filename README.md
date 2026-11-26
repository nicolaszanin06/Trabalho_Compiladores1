# Java Procedural — Analisador Léxico e Sintático

Projeto em desenvolvimento para Compiladores 1 (UnB), usando Flex, Bison e GCC para analisar um subconjunto procedural de Java. A versão atual analisa a entrada e oferece geração de código intermediário de pilha.

## Compilar e executar no WSL/Linux

Dependências: GCC, Make, Flex, Bison e biblioteca do Flex. No Ubuntu:

```bash
sudo apt update
sudo apt install build-essential flex bison libfl-dev
```

A partir da raiz do repositório:

```bash
cd src
make -B
./jtoc < ../testes/validos/declaracoes.java
```

O Makefile está em `src/`. `make -B` regenera os arquivos do Flex/Bison e recompila o executável, inclusive quando os arquivos gerados já vieram do Git.

Para demonstrar um erro:

```bash
./jtoc < ../testes/invalidos/string_nao_encerrada.java
```

Dentro de `src/`, `make clean` remove os arquivos gerados e o executável.

## Gramática implementada

A entrada é uma sequência de comandos, sem envolver o código em `public class` ou `main`:

```java
int x = 0;
while (x < 3) {
    System.out.println(x);
    x++;
}
```

O parser aceita:

- Declarações de `int`, `float`, `char`, `boolean` e `String`, com ou sem inicialização.
- Atribuições simples e compostas (`+=`, `-=`, `*=`, `/=`), incremento e decremento como comandos.
- `if` com ou sem `else` e `while`, com blocos entre chaves.
- `for` com inicialização, condição e atualização opcionais, e corpo entre chaves.
- `switch` com `case`, `default`, queda entre casos e `break`.
- `System.out.print` e `System.out.println`, com uma expressão.
- Expressões com identificadores, literais inteiros, decimais, caracteres, strings e booleanos; operadores aritméticos, relacionais e lógicos; parênteses; `Math.sqrt` e `Math.pow`.

O scanner reconhece outros tokens, como `public`, `class`, `static`, `void`, `final`, `byte`, `short`, `long`, `double`, `do`, `continue`, `return` e `null`. Reconhecer um token não significa que o parser já aceite construções que o utilizam. Classes, métodos e esses comandos/tipos adicionais ainda não fazem parte da gramática.

No cabeçalho do `for`, a inicialização aceita uma declaração ou atribuição; a atualização aceita uma atribuição, incremento ou decremento. Listas separadas por vírgulas não estão implementadas. O `switch` usa a forma tradicional com `:`, seletor `int`, casos constantes inteiros distintos e até um `default`.

Espaços e comentários de linha e bloco são ignorados. Strings e caracteres não podem conter quebras de linha reais; sequências de escape continuam sendo reconhecidas.

## Resultado e erros

Uma entrada válida imprime:

```text
Analise concluida com sucesso! Nenhum erro sintatico encontrado.
```

Por padrão, apenas a mensagem de sucesso é impressa. Para exibir também a AST e
a tabela de símbolos com tipo, valor de inicialização e escopo, use `--debug`:

```bash
./jtoc --debug < ../testes/validos/declaracoes.java
./jtoc --debug ../testes/validos/declaracoes.java
```

As impressões de inspeção estão reunidas em `debug_compiler()`, chamada somente
quando `imprimir_debug` está ativo. A construção da tabela e a análise semântica
são executadas em ambos os modos.

Erros léxicos incluem caracteres inválidos, strings não encerradas e comentários de bloco não encerrados. Erros sintáticos informam a linha e o token próximo da falha. Erros semânticos identificam a variável repetida ou não declarada, ou um `break` fora de laço ou `switch`. A mensagem de sucesso, a AST e a tabela só aparecem quando não há erro léxico, sintático ou semântico. O processo retorna `0` no sucesso e `1` em caso de erro.

A tabela rejeita uma variável já declarada no mesmo escopo e verifica os usos em atribuições, incrementos, condições, expressões e impressões. A busca considera o bloco atual e seus ancestrais, respeitando a ordem das declarações. Blocos irmãos não compartilham variáveis. A declaração no cabeçalho de um `for` só é visível dentro do laço; os rótulos de um `switch` compartilham escopo, a menos que se usem blocos explícitos.

O comando `break` só é permitido dentro de `while`, `for` ou `switch`, inclusive em condicionais e blocos aninhados nessas estruturas. Por exemplo, `while (true) { if (true) { break; } }` é aceito, enquanto `if (true) { break; }` gera `Erro semântico: 'break' fora de laço ou switch.`. A validação também rejeita um `break` depois do fechamento do laço ou `switch`.

## Código intermediário

Use `--ir` para escrever as instruções em stdout após a validação semântica.

```bash
printf 'int x = 3 + 4;\n' | ./jtoc --ir
```

```text
LOAD_CONST 3
LOAD_CONST 4
ADD
STORE x
```

Com `--ir --debug`, a AST e a tabela de símbolos saem em stderr. O modo
`--ir` também verifica inicialização antes da leitura e envia os diagnósticos léxicos a stderr.
As [convenções da IR](docs/codigo-intermediario.md) descrevem os tipos e as instruções.

## Testes e arquivos

Na raiz, com Python 3 e GCC disponíveis, execute:

```bash
python testes/test_analisador.py
```

A suíte cobre os 14 arquivos de entrada, o analisador léxico, a AST, a semântica
e a geração de IR. Com Flex e Bison instalados,
use `python testes/test_analisador.py --regenerate` para regenerar o analisador em
uma pasta temporária antes de testar. O GitHub Actions executa essa opção em cada
push e pull request.

No PowerShell, com GCC disponível no Windows, execute na raiz:

```powershell
.\testes\executar.ps1
```

O script compila os arquivos C gerados e verifica mensagens e códigos de saída. Após alterar `.l` ou `.y`, regenere os arquivos em `src/` antes de usar esse script. Consulte `testes/README.md`.

| Arquivo | Finalidade |
| --- | --- |
| `src/jtoc.l` | Regras léxicas do Flex |
| `src/jtoc.y` | Gramática do Bison, entrada do programa e chamadas dos módulos |
| `src/ast.h` e `src/ast.c` | Estrutura da AST, criação, impressão, liberação e representação textual de expressões |
| `src/tabela_simbolos.h` e `src/tabela_simbolos.c` | Tabela de símbolos, escopos, validação semântica, impressão e liberação |
| `src/gerador_intermediario.h` e `src/gerador_intermediario.c` | Validação estrutural e emissão das instruções de pilha |
| `src/Makefile` | Compilação e limpeza |
| `testes/` | Entradas e verificador |
| `docs/` | Documentação do projeto |
