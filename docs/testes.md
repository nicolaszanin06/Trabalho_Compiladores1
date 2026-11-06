# Testes e validação

## Suíte automatizada

Na raiz do repositório, com Python 3 e GCC no `PATH`, execute:

```bash
python testes/test_analisador.py
```

No Linux, o comando pode ser `python3`. Os 45 testes compilam o analisador e um observador de tokens em uma pasta temporária, sem alterar os arquivos de `src/`. A suíte cobre tokens e diagnósticos, precedência na AST, tabela de símbolos, atribuições compostas, `for`, `switch`, escopos, declaração antes do uso e comportamento com e sem `--debug`.

Os testes de `break` verificam a rejeição fora de laços e `switch`, inclusive depois do fechamento dessas estruturas. Também verificam a aceitação em `while`, `for` e `switch`, com condicionais, blocos e estruturas aninhadas.

Com Flex e Bison instalados, regenere o analisador em uma pasta temporária antes de testar:

```bash
python testes/test_analisador.py --regenerate
```

O GitHub Actions executa essa opção em cada push e pull request. A suíte padrão utiliza os arquivos C gerados versionados no repositório.

## Arquivos de entrada

Além dos casos definidos na suíte Python, há 14 arquivos classificados pelo resultado esperado:

- `testes/validos/`: seis entradas aceitas, incluindo declarações, inicialização, condição com identificador booleano, blocos aninhados, comentários e entrada vazia.
- `testes/invalidos/`: oito entradas rejeitadas, cobrindo ponto e vírgula ausente, expressão ausente, texto solto, caractere inválido, string não encerrada, comentário não encerrado e literais com quebras de linha reais.

As entradas válidas retornam código 0 e imprimem sucesso. A AST e a tabela aparecem somente com `--debug`. Erros léxicos, sintáticos ou semânticos retornam código 1 e impedem sucesso e dumps. Literais com quebra de linha podem gerar erros léxicos e sintáticos na mesma execução.

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
```

Os três últimos exemplos escrevem, respectivamente, `Variável já declarada 'x'`, `Variável não declarada 'x'` e `'break' fora de laço ou switch`, com o prefixo `Erro semântico:` e ponto final, em stderr. Eles deixam stdout vazio mesmo com `--debug`.

## Verificação no Windows e limites

No PowerShell, com GCC instalado no Windows, execute `./testes/executar.ps1` a partir da raiz. Esse verificador cobre os 14 arquivos com `--debug`, comparando stdout, stderr e código de saída. Os casos adicionais de semântica e contexto de `break` são cobertos pela suíte Python. Alterações em `jtoc.l` ou `jtoc.y` exigem regenerar os arquivos C antes de usar o script PowerShell.

A aceitação indica aprovação nas verificações atuais; compatibilidade de tipos, inicialização antes da leitura e constantes/unicidade dos rótulos de `switch` ainda não são verificadas. O analisador não executa a entrada nem gera C. O roteiro completo e a descrição de cada arquivo estão no [README de testes no repositório](https://github.com/nicolaszanin06/Trabalho_Compiladores1/blob/develop/testes/README.md).
