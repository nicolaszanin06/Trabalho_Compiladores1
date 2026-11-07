# **Sobre**

Este projeto foi desenvolvido no contexto da disciplina de Compiladores, com o objetivo de aplicar, na prática, os principais conceitos envolvidos na construção de um compilador. Ao longo do projeto, está sendo desenvolvido um compilador capaz de realizar a tradução de programas escritos em Java para a linguagem C, passando pelas diferentes etapas necessárias para analisar, interpretar e transformar o código-fonte. Esta documentação apresenta de forma organizada as etapas de desenvolvimento do projeto, suas especificações, decisões de implementação, estruturas utilizadas e resultados obtidos, servindo como material de apoio para compreender o funcionamento do compilador e registrar o processo de construção da solução.

---

## **Estado atual**

O analisador usa Flex, Bison e GCC para validar um subconjunto procedural de Java. A versão atual constrói a AST e a tabela de símbolos, verifica declarações e visibilidade de variáveis e rejeita `break` fora de `while`, `for` ou `switch`. A execução ainda não gera código C nem executa o programa de entrada.

A entrada é uma sequência de comandos, sem classe ou método `main`. A AST e a tabela só são impressas com `--debug`. A suíte Python contém 45 testes, incluindo subcasos de sintaxe, tokens, escopos e contexto de `break`.

Consulte [Como rodar](como-rodar.md), [Escopo do projeto](escopo.md), [Analisador sintático](analisador-sintatico.md) e [Testes e validação](testes.md).

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
