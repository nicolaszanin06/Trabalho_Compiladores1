# Testes do analisador

## Suíte automatizada (Windows e Linux)

Requisitos: Python 3 e GCC no `PATH`, sem pacotes adicionais de Python.
Na raiz do repositório, execute:

```powershell
python testes/test_analisador.py
```

No Linux, o comando pode ser `python3 testes/test_analisador.py`.
A suíte compila o analisador e um observador de tokens (`lexer_driver.c`) em uma
pasta temporária, removida ao terminar. O observador chama `yylex()` diretamente
e verifica nomes dos tokens, lexemas, linhas e o lexema inteiro armazenado em
`yylval.stringValue`, incluindo zeros à esquerda.

Os 22 testes incluem subcasos para todos os 14 arquivos abaixo e verificam:

- Palavras reservadas, identificadores, operadores, delimitadores e literais.
- Comentários, espaços, CRLF e linhas dos diagnósticos.
- Tokens de `String`, impressão e funções matemáticas.
- Aceitação ou rejeição das entradas e os códigos de saída 0 ou 1.
- Ausência da mensagem de sucesso e de AST quando há erros.
- Precedência e parênteses na AST.
- Tipo, inicialização e escopo na tabela de símbolos, incluindo nomes repetidos
  em blocos distintos e preservação do valor inicial após uma atribuição.
- Arquivo inexistente e construções da gramática atual, como `while` e `if/else`.

Por padrão, a suíte compila os arquivos C versionados em `src/`. Ao alterar
`jtoc.l` ou `jtoc.y`, instale Flex e Bison e execute:

```powershell
python testes/test_analisador.py --regenerate
```

Essa opção regenera os arquivos em uma pasta temporária, sem alterar `src/`.
O fluxo `.github/workflows/testes.yml` usa essa opção em cada push e pull request.
Qualquer teste que falhe faz o comando e a verificação no GitHub falharem.
A suíte verifica a AST e a tabela de declarações, mas não implica validação
semântica de tipos ou tradução para C.

## Arquivos de entrada

Todos os arquivos de entrada usam `.java` e são classificados pelo resultado esperado no analisador atual.

| Pasta | Resultado esperado |
| --- | --- |
| `validos/` | Sem erros léxicos ou sintáticos; mensagem de sucesso; código 0 |
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

## Roteiro de apresentação no WSL

A partir da raiz do projeto:

```bash
cd src
make -B

# Dois exemplos corretos
./jtoc < ../testes/validos/declaracoes.java
./jtoc < ../testes/validos/ifs_aninhados.java

# Dois erros sintáticos
./jtoc < ../testes/invalidos/sem_ponto_e_virgula.java
./jtoc < ../testes/invalidos/expressao_ausente.java

# Dois erros léxicos
./jtoc < ../testes/invalidos/caractere_invalido.java
./jtoc < ../testes/invalidos/string_nao_encerrada.java
```

## Verificação automática no Windows

No PowerShell, com GCC disponível no Windows, execute na raiz:

```powershell
.\testes\executar.ps1
```

O script verifica os 14 arquivos, comparando as mensagens em stdout/stderr e o código de saída. Nos válidos, também exige os cabeçalhos de AST e tabela de símbolos. Ele compila os arquivos C já gerados em `src/`. Se alterar `jtoc.l` ou `jtoc.y`, regenere os arquivos com Flex/Bison antes de executar o script.

“Válido” significa aceito pela análise léxica e sintática atual. A análise semântica, incluindo declaração de variáveis e compatibilidade de tipos, ainda não está implementada. As entradas são comandos do subconjunto procedural, sem classe ou método `main`.
