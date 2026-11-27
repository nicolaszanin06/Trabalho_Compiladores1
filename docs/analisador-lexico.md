# Analisador léxico

O Flex gera o scanner a partir de `src/jtoc.l`. O cabeçalho `jtoc.tab.h` fornece os tokens do Bison e inclui `ast.h` para compartilhar o tipo da AST. `%option yylineno` conta linhas; `nounput` e `noinput` desabilitam funções não utilizadas. A função `yywrap` está definida em `jtoc.y`.

Identificadores usam `[a-zA-Z_][a-zA-Z0-9_]*`, inteiros usam `[0-9]+` e decimais usam `[0-9]+\.[0-9]+`. Strings e caracteres reconhecem sequências de escape, mas não aceitam quebras de linha reais dentro do literal.

Espaços e comentários de linha são descartados. Comentários de bloco usam o estado exclusivo `COMENTARIO`: o scanner descarta seu conteúdo até encontrar `*/`. Se o arquivo termina antes, informa comentário não encerrado na linha de abertura.

O Flex escolhe a regra que consome mais caracteres. Em caso de empate, escolhe a primeira regra no arquivo.

O scanner registra erros para strings não encerradas, comentários de bloco não encerrados e caracteres inválidos. O contador `erros_lexicos` impede que o programa anuncie sucesso após uma falha léxica. A regra de string não encerrada não consome a quebra de linha seguinte, preservando a linha do diagnóstico.

O scanner reconhece os operadores de atribuição composta `+=`, `-=`, `*=` e `/=`, além de `:` para os rótulos de `switch`. Reconhecimento de tokens e suporte sintático são etapas distintas: palavras-chave como `class`, `do`, `continue` e `return` ainda não têm regras na gramática.

Após a análise sintática, o módulo `tabela_simbolos.c` valida declarações, visibilidade de variáveis, tipos e contexto de `break` na AST. A tradução para C já está implementada e pode ser solicitada com `--c` ou `-o`.

## Contrato dos lexemas

| Categoria | Forma e limites |
| --- | --- |
| Identificadores | ASCII, sensíveis a maiúsculas; não admitem `$`, acentos ou dígito inicial. |
| Inteiros | Base decimal, inclusive `007`; o sinal é token separado e a faixa é validada pela semântica. |
| Decimais | Exigem dígitos dos dois lados do ponto; `.5`, `1.`, expoentes, sufixos `f`/`d`, hexadecimal e separadores `_` não compõem um literal numérico suportado. |
| Chamadas nativas | `System.out.print`, `System.out.println`, `Math.sqrt` e `Math.pow` são tokens completos; inserir espaços no nome pontuado impede o reconhecimento dessa chamada. |
| Caracteres e strings | Sem quebra de linha real; escapes aceitos: `\b`, `\t`, `\n`, `\f`, `\r`, `\"`, `\'` e `\\`. Escapes octais e Unicode não são aceitos. |

O scanner preserva o lexema dos literais em `yylval.stringValue`, sem convertê-los para números. `strtoll` e `strtof` entram na etapa [semântica](analisador-semantico.md). Literais fechados com escapes fora da lista recebem diagnóstico de escape inválido. Caracteres malformados também podem ser reportados pela regra genérica de caractere inválido; não há uma mensagem específica para toda forma de literal `char` incorreto.

Comentários de bloco não são aninhados e terminam no primeiro `*/`. O scanner pode continuar após descartar um caractere inválido; o contador de erros impede sucesso ou geração, mesmo quando o restante é sintaticamente válido.

No modo padrão, os diagnósticos léxicos são enviados a stdout; em `--ir`, `--c` e `-o`, a stderr, reservando stdout para código. [Sintaxe](analisador-sintatico.md) delimita quais tokens são efetivamente aceitos.

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação da documentação inicial | 22/09/2026 | [Henrique](https://github.com/henryqma) |
| 2.0 | Atualização do desenvolvimento para o P2 | 06/11/2026 | [Nicolas](https://github.com/nicolaszanin06) |
