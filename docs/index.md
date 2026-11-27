# Java Procedural → C

**Compiladores 1 (FGA0003 — UnB) — Grupo 7.** Projeto final de um compilador que traduz um subconjunto procedural de Java para C11, com Flex, Bison e módulos em C.

**[Documentação publicada no GitHub Pages](https://nicolaszanin06.github.io/Trabalho_Compiladores1/)**

## Estado final e uso

O `jtoc` realiza análise léxica e sintática, constrói a AST filho-irmão e a tabela de símbolos, valida escopos, tipos, constantes e contexto de `break`. A partir da AST anotada, oferece duas saídas alternativas: IR textual de pilha (`--ir`) e programa C11 (`--c` ou `-o`). A geração C não passa pela IR textual. Nenhum modo executa a entrada automaticamente.

```bash
make -B -C src
./src/jtoc --debug testes/validos/declaracoes.java
./src/jtoc --ir testes/validos/declaracoes.java
./src/jtoc -o programa.c testes/validos/declaracoes.java
gcc -std=c11 -Wall -Wextra -Wpedantic programa.c -o programa -lm
./programa
```

Os comandos pressupõem WSL/Linux e dependências instaladas. A entrada é uma sequência de comandos sem classe ou `main`. O destino de `-o` deve ser novo. O modo padrão valida nomes e tipos, mas **somente os modos de geração verificam inicialização antes da leitura**. `--debug` exibe AST e tabela em stdout no modo padrão e em stderr na geração.

## Guia de leitura

- [Como rodar](como-rodar.md): instalação, compilação, CLI e execução.
- [Escopo](escopo.md) e [Arquitetura e decisões](arquitetura-decisoes.md): linguagem, pipeline, escolhas e limitações.
- [Léxico](analisador-lexico.md), [Sintaxe](analisador-sintatico.md), [AST e tabela](ast-tabela.md) e [Semântica](analisador-semantico.md): front-end.
- [Código intermediário](codigo-intermediario.md) e [Geração final de C](geracao-c.md): contratos das saídas alternativas.
- [Testes e validação](testes.md): suítes Python, executor de IR e testes com GCC.
- [Organização, equipe e sprints](organizacao-sprints.md): “Todos fazem tudo”, frentes e marcos 0–13; [Planejamento](planejamento.md) apresenta a síntese.

## **Integrantes**

<table> 
  <tr> 
    <td align="center"><a href="https://github.com/henryqma"><img style="border-radius: 100%;" src="https://github.com/henryqma.png" width="125px;" alt=""/><br /><sub><b>Henrique Alencar</b></sub></a></td> 
    <td align="center"><a href="https://github.com/nicolaszanin06"><img style="border-radius: 100%;" src="https://github.com/nicolaszanin06.png" width="125px;" alt=""/><br /><sub><b>Nicolas Zanin</b></sub></a></td> 
     <td align="center"><a href="https://github.com/TiagoVieira-596"><img style="border-radius: 100%;" src="https://github.com/TiagoVieira-596.png" width="125px;" alt=""/><br /><sub><b>Tiago Vieira</b></sub></a></td> 
    <td align="center"><a href="https://github.com/ViniGvdo"><img style="border-radius: 100%;" src="https://github.com/ViniGvdo.png" width="125px;" alt=""/><br /><sub><b>Vinicius Gavaldao</b></sub></a></td>
    <td align="center"><a href="https://github.com/warlleymedeiros"><img style="border-radius: 100%;" src="https://github.com/warlleymedeiros.png" width="125px;" alt=""/><br /><sub><b>Warlley Medeiros</b></sub></a></td>
  </tr> 
</table>

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação da documentação inicial | 22/09/2026 | [Henrique](https://github.com/henryqma) |
| 2.0 | Atualização do desenvolvimento para o P2 | 06/11/2026 | [Nicolas](https://github.com/nicolaszanin06) |
| 3.0 | Atualização do estado atual para a entrega final | 27/11/2026 | [Henrique](https://github.com/henryqma) |
| 4.0 | Auditoria técnica e revisão para a entrega final | 27/11/2026 | Grupo 7 |
