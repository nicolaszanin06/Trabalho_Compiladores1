# Árvore Sintática Abstrata (AST) e Tabela de Símbolos

## Árvore Sintática Abstrata (AST)


A implementação da AST no nosso projeto está concentrada nos arquivos `src/ast.c` e `src/ast.h`.

### Como funciona no `jtoc`:
* **Nós (Nodes):** Cada operação (ex: atribuição `=`, soma `+`, condição `if`) é representada como um nó interno na árvore.
* **Folhas (Leaves):** Variáveis (identificadores) e valores literais (números, strings, caracteres) ocupam as folhas da árvore.
* **Integração com o Bison:** Durante a análise sintática (`jtoc.y`), toda vez que uma regra de produção é reduzida com sucesso, uma função de criação de nó do `ast.c` é chamada. Esses nós são então vinculados para formar a árvore completa do programa.

Esta estrutura hierárquica facilita muito a travessia do código durante a otimização e a tradução final para a linguagem C.

## Tabela de Símbolos

A gestão dessa estrutura é feita pelos módulos `src/tabela_simbolos.c` e `src/tabela_simbolos.h`.

### Atribuições da Tabela de Símbolos:
1. **Armazenamento de Metadados:** Guarda o nome da variável, seu tipo de dado associado (`int`, `float`, `char`, etc.) e a linha em que foi declarada.
2. **Gerenciamento de Escopo:** Controla a visibilidade das variáveis. Variáveis declaradas dentro de um bloco `{ ... }` ou de um `if` possuem seu escopo respeitado, sendo removidas ou ocultadas ao fim do bloco.
3. **Apoio à Análise Semântica:** A Tabela de Símbolos é consultada a todo momento para evitar erros comuns, tais como:
    * Uso de variáveis não declaradas previamente.
    * Múltiplas declarações de uma mesma variável no mesmo escopo.
    * Verificação de compatibilidade de tipos em operações (Type Checking).

A sinergia entre a AST e a Tabela de Símbolos é o que permite ao `jtoc` entender não apenas como o código está escrito, mas o que ele significa.

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação do documento | 07/11/2026 | [Henrique](https://github.com/henryqma) |