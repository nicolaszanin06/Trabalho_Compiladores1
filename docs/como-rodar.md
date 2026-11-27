# Como rodar

No WSL/Ubuntu, instale as ferramentas se necessário:

```bash
sudo apt update
sudo apt install build-essential flex bison libfl-dev
```

O executável não é versionado; é necessário compilar antes de usar. O Makefile
requer GNU Make 4.3 ou superior. A partir da raiz do repositório:

```bash
make -B -C src
./src/jtoc < testes/validos/declaracoes.java
./src/jtoc < testes/invalidos/string_nao_encerrada.java
printf '%s\n' 'int x = -2147483648; System.out.println(x);' | ./src/jtoc --ir
printf '%s\n' 'int x = 10; System.out.println(x + 5);' | ./src/jtoc -o programa.c
gcc -std=c11 -Wall -Wextra -Wpedantic programa.c -o programa -lm && ./programa
```

`make -B` regenera os arquivos e recompila mesmo que o Git já contenha os arquivos gerados. O analisador também aceita o caminho de entrada como argumento: `./jtoc ../testes/validos/declaracoes.java`. Para exibir a AST e a tabela de símbolos, use `./jtoc --debug ../testes/validos/declaracoes.java`; essas impressões ficam desativadas por padrão.

Os exemplos com `./jtoc` pressupõem que você entrou em `src/`. O build
paralelo também é suportado: `make -B -j4 -C src`, na raiz, gera o par de
arquivos do Bison em uma única execução.

A entrada deve conter comandos da gramática atual, sem `public class` ou `main`. O programa verifica a sintaxe, declarações e usos de variáveis, compatibilidade de tipos e o contexto de `break`, podendo também emitir código intermediário (`--ir`) ou gerar código C compilável (`--c` ou `-o`).

O sucesso retorna código `0`. Erros léxicos, sintáticos ou semânticos retornam `1` e impedem a mensagem de sucesso ou a geração de código. A análise semântica é executada com ou sem `--debug`. No Bash, consulte o código com `echo $?` imediatamente após executar o analisador.

Por padrão, o programa imprime a mensagem de sucesso e não verifica
inicialização antes da leitura. Com `--ir` ou `--c`, faz essa verificação adicional,
emite o código gerado (IR ou C) em `stdout` e envia diagnósticos a `stderr`.
Com `--ir --debug` ou `--c --debug`, a AST e a tabela de símbolos também ficam em `stderr`.

Para gravar o código C diretamente em arquivo, utilize `-o <saida.c>` (com ou sem `--c`); por segurança, o compilador não sobrescreve um arquivo de destino já existente. As opções `--ir` e `--c` são mutuamente exclusivas, e `-o` não pode ser combinado com `--ir`.

Inteiros devem caber entre `-2147483648` e `2147483647`; o mínimo é emitido
como uma constante única. Strings e caracteres aceitam apenas os escapes
`\b`, `\t`, `\n`, `\f`, `\r`, `\"`, `\'` e `\\`. Um escape como `\q`
provoca erro léxico e impede a geração de IR ou de código C.

Para demonstrar a validação de `break` no Bash, dentro de `src/`:

```bash
printf '%s\n' 'while (true) { break; }' | ./jtoc
echo $? # 0: break dentro de um laço
printf '%s\n' 'if (true) { break; }' | ./jtoc
echo $? # 1: break fora de laço ou switch
```

O segundo exemplo escreve `Erro semântico: 'break' fora de laço ou switch.` em stderr. Nenhum dump ou mensagem de sucesso é impresso, mesmo com `--debug`.

No PowerShell, com GCC instalado no Windows, rode `./testes/executar.ps1` na
raiz. Esse script verifica os 14 arquivos de entrada, cinco casos com `--ir` e
um teste ponta a ponta de geração de código C, compilação e execução usando os
arquivos C já gerados; alterações em Flex/Bison precisam ser regeneradas antes.
Para executar as suítes completas em Python (incluindo o executor de IR e os
testes de compilação e execução em C), rode `python testes/test_analisador.py` e
`python testes/test_gerador_c.py` na raiz. No WSL/Linux, use `python3`.
Veja [Testes e validação](testes.md) e [Geração de código C](geracao-c.md).

Para limpar os arquivos gerados, execute `make clean` dentro de `src/`.

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação da documentação inicial | 22/09/2026 | [Henrique](https://github.com/henryqma) |
| 2.0 | Atualização do desenvolvimento para o P2 | 06/11/2026 | [Nicolas](https://github.com/nicolaszanin06) |
| 3.0 | Adição das instruções de geração e compilação de código C para a entrega final | 27/11/2026 | [Henrique](https://github.com/henryqma) |