# AST e tabela de símbolos

## Modelo filho-irmão

`src/ast.h` e `src/ast.c` definem `NoAST`, suas funções de criação, ligação, impressão e liberação. Cada nó guarda `texto`, o primeiro `filho` e o próximo `irmao`. Ações do Bison criam nós nas reduções; a raiz é um `Bloco`, inclusive para entrada vazia. Essa estrutura preserva a hierarquia necessária à análise e às duas gerações de código. Não há etapa de otimização.

| Campo | Função |
| --- | --- |
| `identificador` | Distingue nomes de variáveis de literais e rótulos estruturais, mesmo com nomes como `If` ou `Bloco`. |
| `tipo` | Tipo resolvido pela análise semântica. |
| `id_escopo`, `id_simbolo` | Ligam cada uso à declaração visível; alimentam os nomes da IR e do C. |
| `valor_caso` | Valor `int32_t` de um rótulo `case` validado. |
| `inteiro_minimo` | Marca `-2147483648` para emissão como constante única. |

Uma `Declaracao` guarda tipo, identificador e inicializador opcional. `For` guarda inicialização, condição, atualização e corpo; cláusulas omitidas são nós `Vazio`. `Switch` guarda seletor e lista `Casos`; `Case` guarda expressão e bloco de comandos, enquanto `Default` guarda apenas comandos. Esses blocos de casos não abrem escopos implicitamente.

## Símbolos e resolução

`src/tabela_simbolos.h` e `src/tabela_simbolos.c` mantêm uma lista encadeada `Simbolo`, com nome, tipo, texto do inicializador, classificação global/local, ID do escopo e ID único da declaração. O campo `valor` registra a expressão inicial **como texto**, sem executá-la nem atualizar-se após atribuições. A tabela impressa não mostra valores correntes de execução.

Os pais dos escopos ficam na pilha de chamadas durante a visita; a tabela conserva os IDs. A busca sobe do escopo atual aos ancestrais e respeita a ordem das declarações. Cada bloco abre escopo; `for` abre um para o cabeçalho e outro para o corpo; os rótulos de um `switch` compartilham seu escopo, salvo chaves explícitas.

Redeclaração no mesmo escopo é erro; nomes iguais em escopos distintos, inclusive aninhados, são permitidos pelo subconjunto. Ao sair de um bloco, as entradas continuam na tabela para inspeção, mas deixam de ser encontradas pela cadeia de escopos ativa. A liberação da tabela ocorre ao fim da compilação.

## Memória e inspeção

`criar_no` copia o texto; `criar_folha` também libera a string recebida do scanner. Filhos ligados passam a pertencer à árvore. `liberar_ast` libera filhos e irmãos; os `%destructor` do Bison liberam tokens e subárvores descartados em erros sintáticos. `texto_expressao` devolve texto alocado ou `NULL` em falha e libera resultados parciais, comportamento exercitado pelo driver de alocação dos testes.

`debug_compiler()` centraliza `imprimir_ast` e `imprimir_tabela`, chamadas apenas com `--debug` após sucesso. A tabela mostra símbolo, tipo, texto da inicialização e classificação do escopo; os IDs são usados internamente, sem aparecer nesse dump.

Veja [Sintaxe](analisador-sintatico.md), [Semântica](analisador-semantico.md), [Arquitetura](arquitetura-decisoes.md) e [Testes](testes.md).

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação do documento | 07/11/2026 | [Henrique](https://github.com/henryqma) |
| 2.0 | Auditoria de metadados, escopos e memória | 27/11/2026 | Grupo 7 |
