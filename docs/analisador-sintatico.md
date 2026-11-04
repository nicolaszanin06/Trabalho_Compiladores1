# Analisador sintático

O Bison gera o parser a partir de `src/scanner.y`. O Flex retorna os tokens definidos no cabeçalho gerado `scanner.tab.h`.

A regra inicial `programa` aceita uma sequência de comandos, inclusive vazia. A gramática atual reconhece:

- Declarações de `int`, `float`, `char`, `boolean` e `String`, com ou sem inicialização.
- Atribuições, incremento e decremento.
- `if` com ou sem `else` e `while`, exigindo blocos entre chaves.
- Chamadas a `System.out.print` e `System.out.println` com uma expressão.
- Expressões com literais, identificadores, parênteses, operadores aritméticos, relacionais e lógicos, menos unário, `Math.sqrt` e `Math.pow`.

A precedência vai de `OR`, `AND`, igualdade, comparação, soma/subtração, multiplicação/divisão/resto até os operadores unários. As declarações `%left` e `%right` definem associatividade.

Classes completas, métodos, `for`, `do`, `switch`, `return`, `break`, `continue`, `final`, `null` e tipos adicionais reconhecidos pelo scanner ainda não têm regras correspondentes no parser.

Não há avaliação de expressões, análise semântica, construção explícita de AST ou geração de código nesta etapa. As ações das regras estão vazias. Aceitar `x = true + 1;` significa apenas que a forma da expressão está na gramática; não verifica tipos nem a declaração de `x`.

`yyerror` informa linha e token perto da falha sintática. O programa verifica o resultado de `yyparse` e o contador de erros léxicos antes de imprimir sucesso. Retorna `0` em caso de sucesso e `1` em caso de erro.
