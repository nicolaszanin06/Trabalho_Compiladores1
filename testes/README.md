# Testes do analisador

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
./scanner < ../testes/validos/declaracoes.java
./scanner < ../testes/validos/ifs_aninhados.java

# Dois erros sintáticos
./scanner < ../testes/invalidos/sem_ponto_e_virgula.java
./scanner < ../testes/invalidos/expressao_ausente.java

# Dois erros léxicos
./scanner < ../testes/invalidos/caractere_invalido.java
./scanner < ../testes/invalidos/string_nao_encerrada.java
```

## Verificação automática no Windows

No PowerShell, com GCC disponível no Windows, execute na raiz:

```powershell
.\testes\executar.ps1
```

O script verifica os 14 arquivos, comparando as mensagens em stdout/stderr e o código de saída. Ele compila os arquivos C já gerados em `src/`. Se alterar `scanner.l` ou `scanner.y`, regenere os arquivos com Flex/Bison antes de executar o script.

“Válido” significa aceito pela análise léxica e sintática atual. A análise semântica, incluindo declaração de variáveis e compatibilidade de tipos, ainda não está implementada. As entradas são comandos do subconjunto procedural, sem classe ou método `main`.
