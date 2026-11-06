# Analisador sintático

O Bison gera o parser a partir de `src/jtoc.y`. O Flex retorna os tokens definidos no cabeçalho gerado `jtoc.tab.h`, que inclui `ast.h` para compartilhar o tipo `NoAST`.

A regra inicial `inicio` guarda a raiz produzida por `programa`, que aceita uma sequência de comandos, inclusive vazia. A gramática atual reconhece:

- Declarações de `int`, `float`, `char`, `boolean` e `String`, com ou sem inicialização.
- Atribuições, incremento e decremento.
- `if` com ou sem `else` e `while`, exigindo blocos entre chaves.
- Chamadas a `System.out.print` e `System.out.println` com uma expressão.
- Expressões com literais, identificadores, parênteses, operadores aritméticos, relacionais e lógicos, menos unário, `Math.sqrt` e `Math.pow`.

A precedência vai de `OR`, `AND`, igualdade, comparação, soma/subtração, multiplicação/divisão/resto até os operadores unários. As declarações `%left` e `%right` definem associatividade.

Classes completas, métodos, `for`, `do`, `switch`, `return`, `break`, `continue`, `final`, `null` e tipos adicionais reconhecidos pelo scanner ainda não têm regras correspondentes no parser.

As ações da gramática constroem a AST usando as funções declaradas em `src/ast.h` e implementadas em `src/ast.c`. Esse módulo contém a estrutura `NoAST`, a criação e ligação dos nós, a impressão, a liberação da árvore e a conversão de expressões em texto. Cada nó guarda um texto, o primeiro filho e o próximo irmão.

A tabela de símbolos permanece em `jtoc.y` e percorre a AST para registrar as declarações. Não há avaliação de expressões, análise semântica ou geração de código nesta etapa. Aceitar `x = true + 1;` significa apenas que a forma da expressão está na gramática; não verifica tipos nem a declaração de `x`.

`yyerror` informa linha e token perto da falha sintática. O programa verifica o resultado de `yyparse` e o contador de erros léxicos antes de imprimir sucesso. Retorna `0` em caso de sucesso e `1` em caso de erro.
