# Testes do analisador

Execute no PowerShell, a partir da raiz do projeto:

```powershell
.\testes\executar.ps1
```

Os arquivos de entrada dos testes usam a extensao `.java`. O script usa `gcc` para compilar os arquivos C ja gerados em `src/` e executa cada arquivo de entrada. Se alterar `scanner.l` ou `scanner.y`, regenere `lex.yy.c` e `scanner.tab.c` com Flex e Bison antes de rodar os testes.

- `validos/`: entradas aceitas pela gramatica atual: arquivo vazio, declaracoes de `int`, `float` e `char`, blocos `if` com literais booleanos e comentarios.
- `invalidos/`: erros sintaticos, caractere invalido e string nao encerrada.

O verificador compara as mensagens em `stdout` e `stderr`. O analisador retorna codigo zero em caso de sucesso e codigo 1 quando ocorre erro lexico ou sintatico. A mensagem de sucesso so aparece quando nenhuma dessas etapas registra erro. O verificador tambem confere o codigo de saida.

A gramatica atual aceita inicializacao de variaveis, expressoes e condicoes com identificadores. Os arquivos atribuicao_na_declaracao.java e condicao_nao_suportada.java, apesar dos nomes e da pasta historica, agora sao verificados como entradas validas. Classes completas continuam fora da gramatica atual.
