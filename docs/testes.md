# Testes e validação

Os 14 casos atuais estão classificados conforme o resultado esperado no analisador:

- `testes/validos/`: seis entradas aceitas, incluindo declarações, inicialização, condição com identificador booleano, blocos aninhados, comentários e entrada vazia.
- `testes/invalidos/`: oito entradas rejeitadas, cobrindo ponto e vírgula ausente, expressão ausente, texto solto, caractere inválido, string não encerrada, comentário não encerrado e literais com quebras de linha reais.

As entradas válidas retornam código 0 e imprimem sucesso. As inválidas retornam código 1 e não imprimem sucesso. Literais com quebra de linha podem gerar erros léxicos e sintáticos na mesma execução.

Para uma demonstração clara, use `declaracoes.java` e `ifs_aninhados.java` como válidos; `sem_ponto_e_virgula.java` e `expressao_ausente.java` como erros sintáticos; `caractere_invalido.java` e `string_nao_encerrada.java` como erros léxicos.

No WSL, compile dentro de `src/` com `make -B` e execute, por exemplo:

```bash
./scanner < ../testes/invalidos/expressao_ausente.java
```

No PowerShell, com GCC instalado no Windows, execute `./testes/executar.ps1` a partir da raiz. O verificador compila os arquivos C gerados e compara stdout, stderr e código de saída de cada caso. Mudanças em Flex/Bison precisam ser regeneradas antes.

A classificação acompanha a gramática atual; não implica validação semântica ou execução do programa. O roteiro completo e a descrição de cada arquivo estão em `testes/README.md`.
