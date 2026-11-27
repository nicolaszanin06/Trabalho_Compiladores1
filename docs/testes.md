# Testes e validação

## Suíte automatizada

Na raiz do repositório, com Python 3, GCC, Make, Flex e Bison disponíveis, compile o executável e execute as suítes nesta ordem:

```bash
make -B -C src
python3 testes/test_analisador.py --regenerate
python3 testes/test_gerador_c.py
```

Se necessário, substitua `python3` por `python`. O build prepara `src/jtoc`, usado pela suíte do backend C. A suíte do analisador compila sua própria cópia e o observador de tokens (`lexer_driver.c`) em pastas temporárias; os programas C gerados pelos testes também são compilados em pastas temporárias.

A suíte é dividida em duas frentes principais:

* **`test_analisador.py` e `executor_ir.py`:** cobrem análise léxica e diagnósticos de tokens, precedência na AST, tabela de símbolos, escopos, compatibilidade de tipos, atribuições compostas, `for`, `switch` (validação de seletor, constantes e unicidade de `case`), contexto de `break`, verificação de inicialização antes da leitura e emissão/simulação de Código Intermediário (`--ir`).
* **`test_gerador_c.py`:** valida a geração de código C (`--c` e `-o`), compilando os arquivos gerados com `gcc -std=c11 -Wall -Wextra -Wpedantic` (com `-lm`) e executando os binários resultantes para comparar `stdout`, `stderr` e códigos de saída, além de testar flags da CLI e rejeição de variáveis não inicializadas.

Os testes de `break` verificam a rejeição fora de laços e `switch`, inclusive depois do fechamento dessas estruturas, e a aceitação em `while`, `for` e `switch` com condicionais, blocos e estruturas aninhadas.

O workflow de testes do GitHub Actions faz o build e executa as duas suítes a cada push e pull request. Apenas `test_analisador.py` recebe `--regenerate`, para regenerar Flex/Bison em sua pasta temporária; sem essa flag, essa suíte utiliza os arquivos C gerados versionados. `test_gerador_c.py` não aceita `--regenerate` e utiliza o executável preparado pelo build.

## Arquivos de entrada

Além dos casos definidos nas suítes Python, há 14 arquivos classificados pelo resultado esperado:

* `testes/validos/`: seis entradas aceitas, incluindo declarações, inicialização, condição com identificador booleano, blocos aninhados, comentários e entrada vazia.
* `testes/invalidos/`: oito entradas rejeitadas, cobrindo ponto e vírgula ausente, expressão ausente, texto solto, caractere inválido, string não encerrada, comentário não encerrado e literais com quebras de linha reais.

As entradas válidas retornam código 0 e imprimem sucesso. A AST e a tabela aparecem somente com `--debug`. Erros léxicos, sintáticos ou semânticos retornam código 1 e impedem sucesso, dumps e geração de código. Literais com quebra de linha podem gerar erros léxicos e sintáticos na mesma execução.

Para uma demonstração clara, use `declaracoes.java` e `ifs_aninhados.java` como válidos; `sem_ponto_e_virgula.java` e `expressao_ausente.java` como erros sintáticos; `caractere_invalido.java` e `string_nao_encerrada.java` como erros léxicos.

## Demonstração no WSL/Linux

A partir da raiz, compile e execute:

```bash
cd src
make -B
./jtoc --debug < ../testes/validos/declaracoes.java
./jtoc < ../testes/invalidos/expressao_ausente.java
./jtoc < ../testes/invalidos/caractere_invalido.java

# Erros semânticos: cada entrada retorna código 1
printf '%s\n' 'int x; int x;' | ./jtoc
printf '%s\n' 'x = 1;' | ./jtoc
printf '%s\n' 'if (true) { break; }' | ./jtoc
echo $? # código da última execução

# Geração de IR e geração/execução de código C
printf '%s\n' 'int a = 3; int b = 4; System.out.println(a + b);' | ./jtoc --ir
printf '%s\n' 'int a = 3; int b = 4; System.out.println(a + b);' | ./jtoc -o /tmp/saida.c
gcc -std=c11 -Wall -Wextra -Wpedantic /tmp/saida.c -o /tmp/saida -lm && /tmp/saida
```

Os três exemplos de erro semântico escrevem, respectivamente, `Variável já declarada 'x'`, `Variável não declarada 'x'` e `'break' fora de laço ou switch`, com o prefixo `Erro semântico:` e ponto final, em stderr. Eles deixam stdout vazio mesmo com `--debug`.

## Verificação no Windows e escopo coberto

No PowerShell, com GCC instalado no Windows, execute `./testes/executar.ps1` a partir da raiz. Esse verificador cobre os 14 arquivos com `--debug` (comparando stdout, stderr e código de saída), cinco casos de geração de código intermediário (`--ir`) e um teste ponta a ponta de geração de código C, compilação via GCC e execução. Alterações em `jtoc.l` ou `jtoc.y` exigem regenerar os arquivos C antes de usar o script PowerShell.

Todas as etapas previstas no escopo do compilador — análise léxica, sintática, semântica (incluindo compatibilidade de tipos, validação de `switch` e inicialização antes da leitura nos modos `--ir` e `--c`), geração de código intermediário e geração de código C compilável — são validadas continuamente pelas suítes automatizadas. O roteiro completo e a descrição de cada arquivo estão no [README de testes no repositório](https://github.com/nicolaszanin06/Trabalho_Compiladores1/blob/develop/testes/README.md).

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação da documentação inicial | 22/09/2026 | [Henrique](https://github.com/henryqma) |
| 2.0 | Atualização do desenvolvimento para o P2 | 06/11/2026 | [Nicolas](https://github.com/nicolaszanin06) |
| 3.0 | Atualização da suíte de testes para a entrega final | 27/11/2026 | [Henrique](https://github.com/henryqma) |