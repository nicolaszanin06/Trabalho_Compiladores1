# Java Procedural — Analisador Léxico e Sintático

Projeto em desenvolvimento para Compiladores 1 (UnB), usando Flex, Bison e GCC para analisar um subconjunto procedural de Java. A versão atual verifica a sintaxe; ainda não executa o programa, não gera C e não faz análise semântica.

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
./scanner < ../testes/validos/declaracoes.java
```

O Makefile está em `src/`. `make -B` regenera os arquivos do Flex/Bison e recompila o executável, inclusive quando os arquivos gerados já vieram do Git.

Para demonstrar um erro:

```bash
./scanner < ../testes/invalidos/string_nao_encerrada.java
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
- Atribuições, incremento e decremento como comandos.
- `if` com ou sem `else` e `while`, com blocos entre chaves.
- `System.out.print` e `System.out.println`, com uma expressão.
- Expressões com identificadores, literais inteiros, decimais, caracteres, strings e booleanos; operadores aritméticos, relacionais e lógicos; parênteses; `Math.sqrt` e `Math.pow`.

O scanner reconhece outros tokens, como `public`, `class`, `static`, `void`, `final`, `byte`, `short`, `long`, `double`, `for`, `do`, `switch`, `case`, `default`, `break`, `continue`, `return` e `null`. Reconhecer um token não significa que o parser já aceite construções que o utilizam. Classes, métodos e esses comandos/tipos adicionais ainda não fazem parte da gramática.

Espaços e comentários de linha e bloco são ignorados. Strings e caracteres não podem conter quebras de linha reais; sequências de escape continuam sendo reconhecidas.

## Resultado e erros

Uma entrada válida imprime:

```text
Analise concluida com sucesso! Nenhum erro sintatico encontrado.
```

Erros léxicos incluem caracteres inválidos, strings não encerradas e comentários de bloco não encerrados. Erros sintáticos informam a linha e o token próximo da falha. A mensagem de sucesso só aparece quando não há erro léxico nem sintático. O processo retorna `0` no sucesso e `1` em caso de erro.

Não há verificação de declaração de variáveis nem de compatibilidade de tipos. Uma entrada aceita sintaticamente não é necessariamente um programa Java semanticamente válido. A tradução para C está planejada para etapas futuras.

## Testes e arquivos

No PowerShell, com GCC disponível no Windows, execute na raiz:

```powershell
.\testes\executar.ps1
```

O script compila os arquivos C gerados e verifica mensagens e códigos de saída. Após alterar `.l` ou `.y`, regenere os arquivos em `src/` antes de usar esse script. Consulte `testes/README.md`.

| Arquivo | Finalidade |
| --- | --- |
| `src/scanner.l` | Regras léxicas do Flex |
| `src/scanner.y` | Gramática do Bison e entrada do programa |
| `src/Makefile` | Compilação e limpeza |
| `testes/` | Entradas e verificador |
| `docs/` | Documentação do projeto |
