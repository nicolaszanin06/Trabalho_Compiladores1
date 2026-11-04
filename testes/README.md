# Testes do analisador

## Suite automatizada (Windows e Linux)

Requisitos: Python 3 e GCC no `PATH`. Nao e necessario instalar pacotes com `pip`.
Na raiz do projeto, execute:

```powershell
python testes/test_analisador.py
```

No Linux, o comando pode ser `python3 testes/test_analisador.py`.
O comando termina com codigo zero quando todos os testes passam e com codigo
diferente de zero quando ha falha. Cada teste tem um nome que identifica o
comportamento verificado; falhas mostram a diferenca entre o esperado e o obtido.

A suite compila dois executaveis em uma pasta temporaria, removida ao terminar:
o analisador completo e um observador de tokens (`lexer_driver.c`). O observador
chama `yylex()` diretamente para testar o lexico independentemente das limitacoes
da gramatica. Os testes verificam o nome do token, o texto reconhecido, a linha e,
nos inteiros, o valor de `yylval.intValue`.

Os 17 testes (alguns com varios subcasos) cobrem:

- Palavras reservadas, identificadores e nomes com prefixos como `intValue`.
- Operadores e delimitadores, incluindo operadores colados e de dois caracteres.
- Literais inteiros, reais, caracteres e strings com escapes.
- Espacos, CRLF, comentarios e contagem de linhas.
- Diagnosticos lexicos e continuidade da leitura depois de caractere invalido.
- Os nove arquivos de exemplo em `validos/` e `invalidos/`.
- Blocos vazios, tokens e linhas exatos nos erros sintaticos e arquivo inexistente.

Por padrao, sao usados os arquivos C ja gerados e versionados em `src/`. Se mudar
`scanner.l` ou `scanner.y`, instale Flex e Bison e execute:

```powershell
python testes/test_analisador.py --regenerate
```

Essa opcao regenera os arquivos em uma pasta temporaria antes de compilar, sem
alterar `src/`. O fluxo `.github/workflows/testes.yml` usa essa opcao em cada push
e pull request. A falha de qualquer teste faz a verificacao do GitHub falhar.

Os testes registram duas limitacoes atuais, e nao as corrigem: erros sintaticos
retornam codigo zero, e erros lexicos podem ser seguidos de uma mensagem de
sucesso. Quando o tratamento de erros for corrigido, as expectativas correspondentes
tambem devem ser atualizadas. A suite nao verifica geracao de codigo ou analise
semantica, pois essas etapas ainda nao estao implementadas.

## Verificador original em PowerShell

Execute no PowerShell, a partir da raiz do projeto:

```powershell
.\testes\executar.ps1
```

Os arquivos de entrada dos testes usam a extensao `.java`. O script usa `gcc` para compilar os arquivos C ja gerados em `src/` e executa cada arquivo de entrada. Se alterar `scanner.l` ou `scanner.y`, regenere `lex.yy.c` e `scanner.tab.c` com Flex e Bison antes de rodar os testes.

- `validos/`: entradas aceitas pela gramatica atual: arquivo vazio, declaracoes de `int`, `float` e `char`, blocos `if` com literais booleanos e comentarios.
- `invalidos/`: erros sintaticos, caractere invalido e string nao encerrada.

O verificador compara as mensagens em `stdout` e `stderr`. Atualmente, o analisador retorna codigo de saida zero mesmo quando informa um erro. Os casos lexicos tambem imprimem uma mensagem de sucesso depois do erro; os testes registram esse comportamento atual para que uma mudanca futura fique visivel.

A gramatica de `scanner.y` ainda nao aceita inicializacao de variaveis, expressoes ou classes completas. Por isso, esses exemplos aparecem apenas como entradas invalidas quando aplicavel.
