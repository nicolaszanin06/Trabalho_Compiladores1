# **Escopo do Projeto**

Esta página detalha os limites e as características da linguagem que o nosso compilador se propõe a processar, bem como as principais decisões de arquitetura e design da equipe.

## **Linguagem Escolhida**

O compilador e analisador léxico estão sendo desenvolvidos para a linguagem **Java**. Contudo, o projeto tem como foco principal trabalhar estritamente com um **subconjunto procedural** desta linguagem.

### Linguagem Alvo e Tradução

O objetivo futuro é um compilador *source-to-source* (transpiler). A implementação atual realiza análise léxica, sintática e semântica inicial, constrói a AST e a tabela de símbolos, mas ainda não traduz para C nem executa a entrada. A análise semântica verifica redeclarações no mesmo escopo, declaração antes do uso, visibilidade de variáveis e contexto de `break`.

A entrada atual é uma sequência de comandos, sem classe ou método `main`. São aceitos os tipos `int`, `float`, `char`, `boolean` e `String`, atribuições simples e compostas, incremento e decremento, `if`/`else`, `while`, `for`, `switch`, impressão e expressões. O corpo de `if`, `while` e `for` exige chaves. `break` só é válido dentro de `while`, `for` ou `switch`, inclusive em blocos aninhados.

Como as linguagens compartilham uma sintaxe semelhante no seu núcleo procedural, o mapeamento será feito de forma direta. A tabela abaixo ilustra a nossa estratégia conceitual de tradução para as futuras etapas de geração de código intermediário e final:

| Java (Origem) | C (Alvo) |
| --- | --- |
| `public static void main()` | `int main(void)` |
| Tipos primitivos (`int`, `float`, `char`) | Equivalentes diretos (`int`, `float`, `char`) |
| `boolean flag = true;` | `int flag = 1;` |
| `final int MAX = 10;` | `const int MAX = 10;` |
| `String texto = "ola";` | `char texto[] = "ola";` |
| Estruturas de controle (`if`, `while`) | Tradução direta de blocos e expressões |

## **Decisões Técnicas**

* **Foco no Essencial:** O projeto foca em criar uma base sólida para as fases de compilação (análise léxica, sintática e semântica), deixando de fora recursos complexos e "pesados" da Orientação a Objetos, como herança ou a diretiva `new`.


* **Ferramentas Clássicas:** Utilização do **Flex** para a geração do *scanner* léxico e do **Bison** para o *parser* sintático, integrados de forma automatizada em um *pipeline* de compilação em C.



---

## **Glossário de Tokens**

O analisador léxico já reconhece os elementos principais do Java procedural. A lista abaixo descreve os *tokens* reconhecidos; isso não significa que todos sejam aceitos pelo parser. Por exemplo, classes, métodos, `do`, `continue`, `return`, `final`, `null` e tipos adicionais ainda não fazem parte da gramática.

### 1. Palavras-Chave e Modificadores

* **Estrutura:** `public`, `class`, `static`, `void`, `final`.


* **Controle de Fluxo:** `if`, `else`, `switch`, `case`, `default`, `while`, `do`, `for`, `break`, `continue`, `return`.



### 2. Tipos de Dados Suportados

* **Primitivos:** `boolean`, `byte`, `char`, `short`, `int`, `long`, `float`, `double`.



### 3. Literais e Identificadores

* **Identificadores:** Nomes de classes, métodos e variáveis (seguindo o padrão `[a-zA-Z_][a-zA-Z0-9_]*`).


* **Numéricos:** Inteiros (`[0-9]+`) e Ponto Flutuante (`[0-9]+\.[0-9]+`).


* **Texto:** Caracteres (`'c'`) e Strings (`"texto"`).


* **Valores fixos:** `true`, `false`, `null`.



### 4. Operadores e Pontuação

* **Aritméticos:** `+`, `-`, `*`, `/`, `%`.


* **Relacionais e Lógicos:** `==`, `!=`, `<`, `<=`, `>`, `>=`, `&&`, `||`, `!`.


* **Atribuição:** `=`, `+=`, `-=`, `*=`, `/=`.

* **Incremento e decremento:** `++`, `--`.


* **Delimitadores:** `;`, `,`, `:`, `(`, `)`, `{`, `}`, `[`, `]`. O ponto aparece nas chamadas reconhecidas como tokens completos (`System.out.print`, `System.out.println`, `Math.sqrt` e `Math.pow`); um ponto isolado é inválido.



### 5. Tratamento de Espaços e Comentários

O analisador foi desenhado para ignorar de forma correta:

* Espaços em branco, quebras de linha (`\n`), tabulações (`\t`) e retorno de carro (`\r`).


* Comentários de linha única (`// ...`) e de múltiplas linhas/bloco (`/* ... */`).


---

## **Limitações**

Para manter o escopo alinhado com os objetivos da disciplina, este compilador **não suporta** os seguintes recursos do Java padrão:

* **Validações ainda pendentes:** Compatibilidade de tipos, inicialização antes da leitura e constantes/unicidade dos rótulos de `switch`. Nomes iguais em escopos diferentes são aceitos.

* **Limites do `for`:** Inicialização por declaração ou atribuição e atualização por atribuição, incremento ou decremento, com cláusulas opcionais. Listas separadas por vírgulas não estão implementadas.

* **Orientação a Objetos avançada:** Diretiva `new`, herança, polimorfismo e classes aninhadas.


* **Tratamento de Exceções:** Palavras-chave como `try`, `catch`, `finally`, `throw` e `throws`.


* **Recursos Modernos e Dinâmicos:** Generics, Lambdas, Streams, Reflection e Threads.



---

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| --- | --- | --- | --- |
| 1.0 | Criação do documento de escopo e definições | 24/09/2026 | [Henrique](https://github.com/henryqma) |

---
