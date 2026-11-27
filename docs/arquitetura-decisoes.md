# Arquitetura, decisões de projeto e limitações

O `jtoc` é um compilador source-to-source do Grupo 7 de Compiladores 1 (FGA0003 — UnB). A unidade de entrada é uma sequência de comandos do subconjunto Java Procedural. As saídas IR e C compartilham o front-end, mas são alternativas independentes.

## Pipeline completo

```text
Código Java Procedural (arquivo ou stdin)
                  |
            Flex: tokens (jtoc.l)
                  |
       Bison: gramática e AST (jtoc.y, ast.*)
                  |
      Análise semântica (tabela_simbolos.*)
      nomes, escopos, tipos, constantes e break
                  |
         AST anotada e tabela de símbolos
                  |
        +---------+---------------------------+
        |                                     |
  Modo padrão                     --ir / --c / -o
  validação básica                inicialização por caminhos
  e sucesso                                   |
                                +-------------+-------------+
                                |                           |
                              --ir                      --c ou -o
                    gerador_intermediario.*            gerador_c.*
                    IR textual de pilha                programa C11
                                                            |
                                                 GCC (etapa externa)
                                                            |
                                                       executável
```

O parser constrói a AST antes da análise semântica; é essa análise que a anota com tipos, símbolos resolvidos, escopos e constantes de `case`. O modo padrão não executa a checagem de inicialização. Nos modos de geração, ela ocorre depois da análise básica e antes da emissão. `--debug` apenas controla a inspeção. Veja [Como rodar](como-rodar.md).

## Por que gerar C diretamente da AST

A implementação de `gerador_c.c` recebe a árvore já anotada e validada. Não lê nem interpreta o texto produzido por `--ir`. A AST mantém blocos, tipos, identidades dos símbolos e estruturas de controle que permitem emitir construções C nativas e aplicar as conversões necessárias.

Traduzir da IR textual exigiria um leitor adicional e reconstruir informações de tipo e estrutura: a IR de pilha não declara tipos de variáveis nem conserva blocos como construções estruturadas. Usar a AST evita essa duplicação e mantém dois produtos didáticos verificáveis: a IR explicita pilha, rótulos e desvios; o backend C produz o programa executável. A IR não é um passo obrigatório entre a análise e o C.

## Delimitações intencionais

**Sem otimização de código.** Conforme a orientação da disciplina, o foco foi correção semântica e tradução fiel para C, sem passes de otimização. Avaliar constantes de `case` e condições para inicialização é uma necessidade da validação; agrupar literais `String` preserva identidade. Essas rotinas não constituem uma etapa geral de otimização.

**Subconjunto estritamente procedural.** A entrada dispensa até a classe externa e o `main` de Java. Classes múltiplas, herança, objetos, métodos de instância ou do usuário, sobrecarga e `return` ficam fora do escopo. Essa delimitação permitiu aprofundar escopos aninhados, curto-circuito, `switch` com fall-through e segurança numérica. Também não há arrays, leitura de entrada, casts, concatenação, `continue`, `do` ou `final`. Reconhecer uma palavra no scanner não implica aceitá-la no parser.

## Soluções técnicas

| Necessidade | Solução implementada |
| --- | --- |
| Inteiros Java de 32 bits | `_Static_assert` exige `int` de 32 bits; auxiliares `jtoc_i32`, `jtoc_iadd`, `jtoc_isub`, `jtoc_imul` e `jtoc_ineg` calculam em `int64_t` e retornam aos 32 bits. |
| Divisão/resto inteiro seguros | `jtoc_idiv` e `jtoc_imod` verificam zero e encerram com erro em stderr. `INT32_MIN / -1` resulta em `INT32_MIN`; o resto resulta em zero. |
| Identidade de variáveis | Cada declaração tem `id_simbolo`; o C usa `jtoc_v<ID>`, evitando colisões com nomes C e preservando homônimos em escopos distintos. |
| Curto-circuito | IR usa saltos e C usa `&&`/`\|\|`; o segundo operando só é avaliado quando necessário. |
| `switch` e `break` | Seletor avaliado uma vez, ordem dos rótulos preservada e saída da estrutura mais próxima; nenhum break é acrescentado entre casos. |
| Identidade de `String` | Literais decodificados de mesmo conteúdo compartilham um array estático; igualdade usa referências. Impressão usa formato `%s` fixo. |
| Saída C íntegra | Geração em temporários antes da publicação; `-o` abre o destino com `wbx` e recusa sobrescrever arquivos. Falhas de escrita ainda podem afetar uma transmissão para stdout. |

## Limitações e diferenças em relação ao Java completo

O alvo usa `float` C, com literais validados por `strtof`; não há suporte a `double`. `Math.sqrt` e `Math.pow` recebem valores convertidos para `float`, chamam a biblioteca em `double` e convertem o resultado para `float`. Domínio, não finitos e operações de ponto flutuante seguem o alvo e sua biblioteca, sem simular exceções Java. A impressão usa `%f` com seis casas, em vez de `Float.toString`.

`char` C representa um byte, não o `char` UTF-16 do Java; o backend rejeita literais com mais de um byte decodificado. Strings preservam bytes UTF-8. Nomes iguais em escopos aninhados são permitidos pelo projeto, inclusive onde Java completo imporia restrições adicionais.

A análise de inicialização é conservadora: não propaga valores de variáveis nem prova condições com `float`; cada rótulo de `switch` é considerado uma possível entrada direta, mesmo com seletor constante. Programas cuja segurança depende dessas provas podem ser rejeitados. O [executor da IR](codigo-intermediario.md) é um auxiliar de testes e não um runtime Java.

Consulte [Escopo](escopo.md), [AST e tabela](ast-tabela.md), [Semântica](analisador-semantico.md), [Geração C](geracao-c.md) e [Testes](testes.md) para os contratos detalhados.
