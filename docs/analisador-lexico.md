# Analisador léxico

O Flex gera o scanner a partir de `src/scanner.l`. O cabeçalho `scanner.tab.h` fornece os tokens do Bison. `%option yylineno` conta linhas; `nounput` e `noinput` desabilitam funções não utilizadas. A função `yywrap` está definida em `scanner.y`.

Identificadores usam `[a-zA-Z_][a-zA-Z0-9_]*`, inteiros usam `[0-9]+` e decimais usam `[0-9]+\.[0-9]+`. Strings e caracteres reconhecem sequências de escape, mas não aceitam quebras de linha reais dentro do literal.

Espaços e comentários de linha são descartados. Comentários de bloco usam o estado exclusivo `COMENTARIO`: o scanner descarta seu conteúdo até encontrar `*/`. Se o arquivo termina antes, informa comentário não encerrado na linha de abertura.

O Flex escolhe a regra que consome mais caracteres. Em caso de empate, escolhe a primeira regra no arquivo.

O scanner registra erros para strings não encerradas, comentários de bloco não encerrados e caracteres inválidos. O contador `erros_lexicos` impede que o programa anuncie sucesso após uma falha léxica. A linha de uma string não encerrada não avança por consumir a quebra de linha seguinte.

Reconhecimento de tokens e suporte sintático são etapas distintas: várias palavras-chave já reconhecidas ainda não têm regras na gramática. A análise semântica e a tradução para C permanecem planejadas.
