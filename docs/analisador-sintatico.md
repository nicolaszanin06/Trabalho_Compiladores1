# Analisador sintático

O Bison gera o parser a partir de `src/jtoc.y`. O Flex retorna os tokens definidos no cabeçalho gerado `jtoc.tab.h`, que inclui `ast.h` para compartilhar o tipo `NoAST`.

A regra inicial `inicio` guarda a raiz produzida por `programa`, que aceita uma sequência de comandos, inclusive vazia. A gramática atual reconhece:

- Declarações de `int`, `float`, `char`, `boolean` e `String`, com ou sem inicialização.
- Atribuições simples e compostas (`+=`, `-=`, `*=`, `/=`), incremento e decremento.
- `if` com ou sem `else` e `while`, exigindo blocos entre chaves.
- `for` com as três cláusulas opcionais e corpo entre chaves.
- `switch` com rótulos `case` e `default`, comandos `break` e blocos explícitos.
- Chamadas a `System.out.print` e `System.out.println` com uma expressão.
- Expressões com literais, identificadores, parênteses, operadores aritméticos, relacionais e lógicos, menos unário, `Math.sqrt` e `Math.pow`.

A precedência vai de `OR`, `AND`, igualdade, comparação, soma/subtração, multiplicação/divisão/resto até os operadores unários. As declarações `%left` e `%right` definem associatividade.

Classes completas, métodos, `do`, `return`, `continue`, `final`, `null` e tipos adicionais reconhecidos pelo scanner ainda não têm regras correspondentes no parser.

As ações da gramática constroem a AST usando as funções declaradas em `src/ast.h` e implementadas em `src/ast.c`. Esse módulo contém a estrutura `NoAST`, a criação e ligação dos nós, a impressão, a liberação da árvore e a conversão de expressões em texto. Cada nó guarda um texto, o primeiro filho e o próximo irmão.

O nó `For` tem quatro filhos: inicialização, condição, atualização e corpo. As cláusulas omitidas geram nós `Vazio`. O nó `Switch` guarda a expressão e uma lista `Casos`; cada `Case` guarda o rótulo e seu bloco de comandos, enquanto `Default` guarda apenas o bloco. `Break` é um comando sem filhos.

A tabela de símbolos permanece em `jtoc.y` e percorre a AST na ordem dos comandos para registrar declarações e validar usos de variáveis. Cada escopo tem um identificador e uma referência ao pai durante a visita. A inserção rejeita nomes repetidos no mesmo escopo, e a busca sobe pelos ancestrais para resolver usos. O campo `identificador` do nó distingue variáveis de literais e dos textos dos operadores.

Cada bloco abre um escopo. O `for` abre um escopo para o cabeçalho e outro para o corpo; os `case` compartilham o escopo do `switch`, salvo blocos explícitos. A análise informa `Variável já declarada` ou `Variável não declarada` em stderr e impede a mensagem de sucesso em caso de erro.

Não há avaliação de expressões, validação de tipos ou geração de código nesta etapa. Por exemplo, `int x; x = true + 1;` passa pelas verificações atuais porque `x` está declarada, mas a compatibilidade de tipos ainda não é verificada. A análise também não verifica inicialização antes da leitura nem todas as restrições de nomes e rótulos do Java.

`yyerror` informa linha e token perto da falha sintática. O programa verifica o resultado de `yyparse` e os contadores de erros léxicos e semânticos antes de imprimir sucesso. Retorna `0` em caso de sucesso e `1` em caso de erro.

As chamadas de `imprimir_ast()` e `imprimir_tabela()` ficam em `debug_compiler()`. O `main` só chama essa função se `imprimir_debug` estiver ativo, por meio da opção `--debug`. Sem essa opção, a análise completa é executada e apenas a mensagem de sucesso é impressa. A montagem da tabela e a validação semântica não dependem da impressão de debug.
