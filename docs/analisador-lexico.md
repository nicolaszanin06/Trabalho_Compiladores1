# Analisador léxico

O Flex gera o scanner a partir de `src/jtoc.l`. O cabeçalho `jtoc.tab.h` fornece os tokens do Bison e inclui `ast.h` para compartilhar o tipo da AST. `%option yylineno` conta linhas; `nounput` e `noinput` desabilitam funções não utilizadas. A função `yywrap` está definida em `jtoc.y`.

Identificadores usam `[a-zA-Z_][a-zA-Z0-9_]*`, inteiros usam `[0-9]+` e decimais usam `[0-9]+\.[0-9]+`. Strings e caracteres reconhecem sequências de escape, mas não aceitam quebras de linha reais dentro do literal.

Espaços e comentários de linha são descartados. Comentários de bloco usam o estado exclusivo `COMENTARIO`: o scanner descarta seu conteúdo até encontrar `*/`. Se o arquivo termina antes, informa comentário não encerrado na linha de abertura.

O Flex escolhe a regra que consome mais caracteres. Em caso de empate, escolhe a primeira regra no arquivo.

O scanner registra erros para strings não encerradas, comentários de bloco não encerrados e caracteres inválidos. O contador `erros_lexicos` impede que o programa anuncie sucesso após uma falha léxica. A linha de uma string não encerrada não avança por consumir a quebra de linha seguinte.

O scanner reconhece os operadores de atribuição composta `+=`, `-=`, `*=` e `/=`, além de `:` para os rótulos de `switch`. Reconhecimento de tokens e suporte sintático são etapas distintas: palavras-chave como `class`, `do`, `continue` e `return` ainda não têm regras na gramática.

Após a análise sintática, o módulo `tabela_simbolos.c` valida declarações, visibilidade de variáveis e contexto de `break` na AST. A tradução para C permanece planejada.
