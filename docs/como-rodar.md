# Como rodar

No WSL/Ubuntu, instale as ferramentas se necessário:

```bash
sudo apt update
sudo apt install build-essential flex bison libfl-dev
```

A partir da raiz do repositório, compile em `src/`, onde fica o Makefile:

```bash
cd src
make -B
./jtoc < ../testes/validos/declaracoes.java
./jtoc < ../testes/invalidos/string_nao_encerrada.java
```

`make -B` regenera os arquivos e recompila mesmo que o Git já contenha os arquivos gerados. O analisador também aceita o caminho de entrada como argumento: `./jtoc ../testes/validos/declaracoes.java`. Para exibir a AST e a tabela de símbolos, use `./jtoc --debug ../testes/validos/declaracoes.java`; essas impressões ficam desativadas por padrão.

A entrada deve conter comandos da gramática atual, sem `public class` ou `main`. O programa verifica a sintaxe, declarações e usos de variáveis e o contexto de `break`. Ele não executa esses comandos nem gera código C.

O sucesso retorna código `0`. Erros léxicos, sintáticos ou semânticos retornam `1` e impedem a mensagem de sucesso. A análise semântica é executada com ou sem `--debug`. No Bash, consulte o código com `echo $?` imediatamente após executar o analisador.

Para demonstrar a validação de `break` no Bash, dentro de `src/`:

```bash
printf '%s\n' 'while (true) { break; }' | ./jtoc
echo $? # 0: break dentro de um laço
printf '%s\n' 'if (true) { break; }' | ./jtoc
echo $? # 1: break fora de laço ou switch
```

O segundo exemplo escreve `Erro semântico: 'break' fora de laço ou switch.` em stderr. Nenhum dump ou mensagem de sucesso é impresso, mesmo com `--debug`.

No PowerShell, com GCC instalado no Windows, rode `./testes/executar.ps1` na raiz. Esse script verifica os 14 arquivos de entrada usando os arquivos C já gerados; alterações em Flex/Bison precisam ser regeneradas antes. Para executar os 45 testes, incluindo as regras semânticas e o contexto de `break`, use `python testes/test_analisador.py` na raiz. Veja [Testes e validação](testes.md).

Para limpar os arquivos gerados, execute `make clean` dentro de `src/`.
