# Organização, equipe e sprints

**Compiladores 1 (FGA0003 — UnB) — Grupo 7.** O projeto traduziu um subconjunto Java Procedural para C, integrando as etapas estudadas na disciplina.

## Metodologia: “Todos fazem tudo”

Durante as aulas e encontros presenciais, a equipe discutiu em conjunto o escopo, as regras da linguagem e a resolução dos problemas. A divisão por frentes indica quem tomou a iniciativa em cada etapa, com apoio mútuo em todos os processos; não representa trabalho exclusivo nem isolamento de conhecimento.

| Integrantes | Frentes lideradas |
| --- | --- |
| Vinicius e Warlley | Implementação da análise léxica, análise sintática e início da análise semântica |
| Nicolas | Implementação da suíte de testes automatizados e consolidação da análise semântica |
| Henrique | Estruturação da documentação e geração final de código em C |
| Tiago | Geração de código intermediário (`--ir`) |

Os perfis dos cinco integrantes estão na [página inicial](index.md#integrantes).

## Sprints como impulsos de implementação

As sprints não tiveram duração fixa rígida ou correspondência obrigatória com semanas fechadas. Foram marcos organizacionais de impulsos de desenvolvimento, com atividades sobrepostas e revisões conjuntas. A linha do tempo abaixo registra o percurso de implementação; a decomposição de 0–10 organiza os temas da etapa inicial, sem atribuir datas ou exclusividade a cada atividade.

| Sprint | Marco e atividades |
| --- | --- |
| 0 | Preparação: configuração do ambiente e do repositório, estudo dos materiais da disciplina e alinhamento inicial do escopo. |
| 1 | Definição e organização do projeto: Java Procedural como origem, C como alvo e colaboração entre integrantes. |
| 2 | Início do léxico: estudo de expressões regulares, Flex, palavras reservadas, operadores e delimitadores. |
| 3 | Consolidação do scanner: identificadores, literais, comentários, espaços e diagnósticos léxicos. |
| 4 | Início da análise sintática: Bison, integração com Flex e automação pelo Makefile. |
| 5 | Estruturação do parser: produções de comandos e declarações, condicionais e tratamento de erros. |
| 6 | Expansão da gramática: expressões e repetição, validação prática e documentação das primeiras etapas. |
| 7 | Testes automatizados iniciais: entradas válidas e inválidas, diagnósticos e execução reproduzível. |
| 8 | AST em C no modelo filho-irmão, construção nas ações da gramática e inspeção da árvore. |
| 9 | Tabela de símbolos: identificadores, tipos, texto do inicializador e controle de escopos. |
| 10 | Início da análise semântica: integração entre AST e tabela, resolução de nomes e checagem de tipos. |
| 11 | Finalização da semântica: escopos, tipagem, constantes, inicialização por caminhos e ampliação dos testes; início da geração intermediária (`--ir`). |
| 12 | Conclusão completa da IR: expressões, conversões, controle de fluxo, curto-circuito, `switch`, impressão e funções matemáticas; validação com executor auxiliar. |
| 13 | Geração final de C (`--c` / `-o`), testes end-to-end com compilação e execução via GCC, validação de limites numéricos e refinamento geral para a entrega final. |

## Resultado final

As etapas convergem no pipeline descrito em [Arquitetura e decisões](arquitetura-decisoes.md). O compilador oferece validação, emissão de IR e geração de C11, com [testes automatizados](testes.md) para análise, fluxo, limites numéricos e execução do programa traduzido. As limitações assumidas pela equipe estão em [Escopo](escopo.md).
