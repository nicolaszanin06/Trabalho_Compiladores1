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
```

`make -B` regenera os arquivos e recompila mesmo que o Git já contenha os arquivos gerados. O analisador também aceita o caminho de entrada como argumento: `./jtoc ../testes/validos/declaracoes.java`. Para exibir a AST e a tabela de símbolos, use `./jtoc --debug ../testes/validos/declaracoes.java`; essas impressões ficam desativadas por padrão.

Os exemplos com `./jtoc` pressupõem que você entrou em `src/`. O build
paralelo também é suportado: `make -B -j4 -C src`, na raiz, gera o par de
arquivos do Bison em uma única execução.

A entrada deve conter comandos da gramática atual, sem `public class` ou `main`. O programa verifica a sintaxe, declarações e usos de variáveis e o contexto de `break`. Ele não executa esses comandos nem gera código C.

O sucesso retorna código `0`. Erros léxicos, sintáticos ou semânticos retornam `1` e impedem a mensagem de sucesso. A análise semântica é executada com ou sem `--debug`. No Bash, consulte o código com `echo $?` imediatamente após executar o analisador.

Por padrão, o programa imprime a mensagem de sucesso e não verifica
inicialização antes da leitura. Com `--ir`, faz essa verificação adicional,
emite instruções em stdout e envia diagnósticos a stderr. Com `--ir --debug`,
a AST e a tabela também ficam em stderr. Não há execução da IR nesse comando.

Inteiros devem caber entre `-2147483648` e `2147483647`; o mínimo é emitido
como uma constante única. Strings e caracteres aceitam apenas os escapes
`\b`, `\t`, `\n`, `\f`, `\r`, `\"`, `\'` e `\\`. Um escape como `\q`
provoca erro léxico e impede a geração de IR.

Para demonstrar a validação de `break` no Bash, dentro de `src/`:

```bash
printf '%s\n' 'while (true) { break; }' | ./jtoc
echo $? # 0: break dentro de um laço
printf '%s\n' 'if (true) { break; }' | ./jtoc
echo $? # 1: break fora de laço ou switch
```

O segundo exemplo escreve `Erro semântico: 'break' fora de laço ou switch.` em stderr. Nenhum dump ou mensagem de sucesso é impresso, mesmo com `--debug`.

No PowerShell, com GCC instalado no Windows, rode `./testes/executar.ps1` na
raiz. Esse script verifica os 14 arquivos de entrada e cinco casos com `--ir`
usando os arquivos C já gerados; alterações em Flex/Bison precisam ser
regeneradas antes. Para executar a suíte completa, incluindo o executor de IR,
use `python testes/test_analisador.py` na raiz. No WSL/Linux, use `python3`.
Veja [Testes e validação](testes.md).

Para limpar os arquivos gerados, execute `make clean` dentro de `src/`.

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação da documentação inicial | 22/09/2026 | [Henrique](https://github.com/henryqma) |
| 2.0 | Atualização do desenvolvimento para o P2 | 06/11/2026 | [Nicolas](https://github.com/nicolaszanin06) |
