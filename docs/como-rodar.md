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
./scanner < ../testes/validos/declaracoes.java
./scanner < ../testes/invalidos/string_nao_encerrada.java
```

`make -B` regenera os arquivos e recompila mesmo que o Git já contenha os arquivos gerados. O scanner também aceita o caminho de entrada como argumento: `./scanner ../testes/validos/declaracoes.java`.

A entrada deve conter comandos da gramática atual, sem `public class` ou `main`. O programa verifica a sintaxe e não executa esses comandos nem gera código C.

O sucesso retorna código `0`. Erros léxicos ou sintáticos retornam `1` e impedem a mensagem de sucesso. No Bash, consulte o código com `echo $?` imediatamente após executar o scanner.

No PowerShell, com GCC instalado no Windows, rode `./testes/executar.ps1` na raiz. Esse script utiliza os arquivos C já gerados; alterações em Flex/Bison precisam ser regeneradas antes.

Para limpar os arquivos gerados, execute `make clean` dentro de `src/`.
