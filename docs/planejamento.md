# **Planejamento e Metodologia**

## **Metodologia de Trabalho**

Para o desenvolvimento deste compilador, a equipe optou por uma abordagem altamente colaborativa. Adotamos um modelo onde todos fazem um pouco de tudo, rotacionando os trabalhos entre os integrantes. Essa escolha garante que todos os membros da equipe adquiram experiência prática em todas as fases da construção do compilador e participem ativamente da resolução de problemas.

---

## **Planejamento das Sprints**

O nosso planejamento foi estruturado para acompanhar as semanas letivas do plano de ensino da disciplina. O projeto foi dividido em 11 Sprints principais:

| Sprint | Período | Objetivo |
| ------ | :-----: | -------- |
| [Sprint 1](#sprint-1) | 10/08/2026 - 16/08/2026 | Definição e Organização |
| [Sprint 2](#sprint-2) | 17/08/2026 - 23/08/2026 | Início da Análise Léxica |
| [Sprint 3](#sprint-3) | 24/08/2026 - 30/08/2026 | Conclusão da Análise Léxica |
| [Sprint 4](#sprint-4) | 31/08/2026 - 06/09/2026 | Início da Análise Sintática |
| [Sprint 5](#sprint-5) | 07/09/2026 - 13/09/2026 | Estruturação do Parser |
| [Sprint 6](#sprint-6) | 14/09/2026 - 27/09/2026 | Conclusão da Análise Sintática |
| Sprint 7 | 05/10/2026 - 11/10/2026 | Integração da Análise Semântica |
| Sprint 8 | 12/10/2026 - 18/10/2026 | Geração de Código Intermediário |
| Sprint 9 | 19/10/2026 - 25/10/2026 | Otimização de Código |
| Sprint 10 | 26/10/2026 - 10/11/2026 | Geração de Código Final |
| Sprint 11 | 16/11/2026 - 29/11/2026 | Implementação Final |

---

## **Sprints Realizadas**

### Sprint 1

**Objetivo:** Definição e Organização

**Período:** 10/08/2026 - 16/08/2026

**Descrição:**

* Decisão da linguagem a ser compilada (Java) e do tipo de projeto (Compilador).
* Configuração do repositório no GitHub e do ambiente de desenvolvimento.

### Sprint 2

**Objetivo:** Início da Análise Léxica

**Período:** 17/08/2026 - 23/08/2026

**Descrição:**

* Estudos práticos iniciais sobre expressões regulares utilizando a ferramenta Flex.
* Implementação inicial do Analisador Léxico, conseguindo reconhecer operadores, algumas palavras reservadas e delimitadores.

### Sprint 3

**Objetivo:** Conclusão da Análise Léxica

**Período:** 24/08/2026 - 30/08/2026

**Descrição:**

* Finalização do Scanner no Flex.
* Inclusão do reconhecimento de tipos de variáveis (int, float, boolean, etc.) e identificadores.
* Implementação de suporte para ignorar espaços em branco e capturar comentários de linha (`//`) e de bloco (`/* */`).
* Inclusão de tratamento de erros léxicos para caracteres inválidos e *strings* não encerradas adequadamente.

### Sprint 4

**Objetivo:** Início da Análise Sintática

**Período:** 31/08/2026 - 06/09/2026

**Descrição:**

* Início do uso da ferramenta Bison para gerar o *parser* sintático, integrado numa compilação automatizada.
* Integração automatizada do Flex e do Bison utilizando um arquivo `Makefile` para organizar a compilação e os testes.

### Sprint 5

**Objetivo:** Estruturação do Parser

**Período:** 07/09/2026 - 13/09/2026

**Descrição:**

* Implementação das primeiras regras sintáticas, como a regra `program` e declarações simples de `command`.
* Implementação parcial do analisador sintático com suporte inicial a 1 operador condicional básico na gramática.
* Configuração do tratamento padrão de erros sintáticos, apontando a linha do erro no código fonte.

### Sprint 6

**Objetivo:** Conclusão da Análise Sintática

**Período:** 14/09/2026 - 27/09/2026

**Descrição:**

* Expansão da análise sintática com a adição do suporte para expressões e de 1 operador de repetição.
* Realização de testes práticos das funcionalidades implementadas pelo terminal..
* Elaboração da documentação.

---

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Documentação do planejamento | 24/09/2026 | [Henrique](https://github.com/henryqma) |