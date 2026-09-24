# Testes do analisador

Execute no PowerShell, a partir da raiz do projeto:

```powershell
.\testes\executar.ps1
```

Os arquivos de entrada dos testes usam a extensao `.java`. O script usa `gcc` para compilar os arquivos C ja gerados em `src/` e executa cada arquivo de entrada. Se alterar `scanner.l` ou `scanner.y`, regenere `lex.yy.c` e `scanner.tab.c` com Flex e Bison antes de rodar os testes.

- `validos/`: entradas aceitas pela gramatica atual: arquivo vazio, declaracoes de `int`, `float` e `char`, blocos `if` com literais booleanos e comentarios.
- `invalidos/`: erros sintaticos, caractere invalido e string nao encerrada.

O verificador compara as mensagens em `stdout` e `stderr`. Atualmente, o analisador retorna codigo de saida zero mesmo quando informa um erro. Os casos lexicos tambem imprimem uma mensagem de sucesso depois do erro; os testes registram esse comportamento atual para que uma mudanca futura fique visivel.

A gramatica de `scanner.y` ainda nao aceita inicializacao de variaveis, expressoes ou classes completas. Por isso, esses exemplos aparecem apenas como entradas invalidas quando aplicavel.
