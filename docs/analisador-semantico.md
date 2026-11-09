# Analisador Semântico

Atualmente, o analisador semântico encontra-se em fase de desenvolvimento, com a infraestrutura base ([AST e Tabela de Símbolos](./ast-tabela.md)) já concluída e integrada.


## O que foi implementado:

A integração inicial ocorre percorrendo a AST gerada pelo Bison (`jtoc.y`). Durante a travessia da árvore, sempre que um nó de claração de variável é encontrado, o compilador invoca as funções da Tabela de Símbolos para registrar o indentificador.

Exemplo:

* O Bison reconhece a regra sintática de declaração: `int contador = 10;`.
* O nó da AST para a declaração é criado.
* O analisador semântico extrai o identificador (`contador`), o tipo (`int`) e o valor (`10`), inserindo-os na Tabela de Símbolos, vinculados ao escopo atual.


## O que falta implementar:

### 1. Checagem e Validação de Escopo

Variáveis não declaradas: Garantir que qualquer identificador lido em uma expressão (ex: `x = y + 2;`) já tenha sido previamente inserido na Tabela de Símbolos no escopo atual ou superior.

Redeclaração de variáveis: Emitir erro caso o código tente declarar uma variável com o mesmo nome no mesmo escopo (ex: `int a; float a;`).

### 2. Checagem de Tipos (Type Checking)

Compatibilidade em Atribuições: Validar se o tipo do valor sendo atribuído a uma variável é compatível com o tipo declarado na Tabela de Símbolos (ex: impedir `int a = "texto"`;).

Validação de Operações: Garantir que operadores matemáticos ou lógicos estejam sendo aplicados a tipos válidos (ex: não permitir soma entre um `boolean` e um `char`).

### 3. Melhoria no Tratamento e Reporte de Erros

Implementar mensagens de erro semântico claras, especificando a linha, a coluna e o motivo do erro (ex: `Erro Semântico na linha 15: Variável 'total' não declarada`).

Adicionar novos testes automatizados no diretório `testes/invalidos/` específicos para capturar essas falhas semânticas e garantir a robustez do compilador antes da etapa de Geração de Código.


## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação do documento | 08/11/2026 | [Henrique](https://github.com/henryqma) |