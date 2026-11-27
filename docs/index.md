# **Sobre**

Este projeto foi desenvolvido no contexto da disciplina de Compiladores, com o objetivo de aplicar, na prática, os principais conceitos envolvidos na construção de um compilador. Ao longo do projeto, foi desenvolvido um compilador capaz de realizar a tradução de programas escritos em Java para a linguagem C, passando pelas diferentes etapas necessárias para analisar, interpretar e transformar o código-fonte. Esta documentação apresenta de forma organizada as etapas de desenvolvimento do projeto, suas especificações, decisões de implementação, estruturas utilizadas e resultados obtidos, servindo como material de apoio para compreender o funcionamento do compilador e registrar o processo de construção da solução.

---

## **Estado atual**

O compilador (`jtoc`) está concluído e utiliza Flex, Bison e GCC para traduzir um subconjunto procedural de Java para a linguagem C. A versão final realiza todas as etapas do fluxo de compilação: análise léxica e sintática, construção da AST e da tabela de símbolos, análise semântica completa (verificando declarações, escopo e visibilidade de variáveis, compatibilidade de tipos, uso de variáveis não inicializadas e contexto de `break` em laços e `switch`), geração de código intermediário (IR) e geração de código C compilável e executável.

A entrada é uma sequência de comandos, sem classe ou método `main`. A AST e a tabela de símbolos só são impressas com `--debug`, enquanto o código gerado em C pode ser compilado e executado diretamente via GCC. A suíte de testes automatizados em Python valida léxico, sintaxe, escopos, semântica de tipos, contexto de `break`, simulação de código intermediário e compilação/execução ponta a ponta do código C gerado.

Consulte [Como rodar](como-rodar.md), [Escopo do projeto](escopo.md), [Analisador sintático](analisador-sintatico.md), [Analisador semântico](analisador-semantico.md), [Código intermediário](codigo-intermediario.md), [Geração de código C](geracao-c.md) e [Testes e validação](testes.md).

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

---

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação da documentação inicial | 22/09/2026 | [Henrique](https://github.com/henryqma) |
| 2.0 | Atualização do desenvolvimento para o P2 | 06/11/2026 | [Nicolas](https://github.com/nicolaszanin06) |
| 3.0 | Atualização do estado atual para a entrega final | 27/11/2026 | [Henrique](https://github.com/henryqma) |