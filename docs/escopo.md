# **Escopo do Projeto**

Esta página detalha os limites e as características da linguagem que o nosso compilador se propõe a processar, bem como as principais decisões de arquitetura e design da equipe.

## **Linguagem Escolhida**

O compilador e analisador léxico foram desenvolvidos para a linguagem **Java**. Contudo, o projeto tem como foco principal trabalhar estritamente com um **subconjunto procedural** desta linguagem.

### Linguagem Alvo e Tradução

O projeto implementa um compilador *source-to-source* (transpiler) completo. O `jtoc` realiza análise léxica, sintática e semântica, constrói a AST e a tabela de símbolos, gera código intermediário (IR) de pilha com `--ir` e traduz o programa para código C compilável com `--c` ou `-o`. Verifica nomes, escopos, tipos, contexto de `break`, impressão, argumentos de `Math.sqrt`/`Math.pow` e casos constantes e únicos de `switch`. Antes de emitir IR ou código C, verifica também inicialização antes da leitura.

A entrada é uma sequência de comandos, sem classe ou método `main`. São aceitos os tipos `int`, `float`, `char`, `boolean` e `String`, atribuições simples e compostas, incremento e decremento, `if`/`else`, `while`, `for`, `switch`, impressão e expressões. O corpo de `if`, `while` e `for` exige chaves. `break` só é válido dentro de `while`, `for` ou `switch`, inclusive em blocos aninhados.

A tabela abaixo resume o mapeamento adotado na geração de código C (detalhado em [Geração de código C](geracao-c.md)). Identificadores declarados recebem nomes únicos (`jtoc_v<id>`) para evitar conflitos de escopo e colisões com palavras reservadas do C:

| Java (Origem) | C (Alvo) |
| --- | --- |
| Sequência de comandos na raiz | Envolvida em `int main(void)` |
| `int`, `float`, `char` | `int32_t`, `float`, `char` |
| `boolean flag = true;` | `bool jtoc_v0 = true;` (`<stdbool.h>`) |
| `String texto = "ola";` | `const char *jtoc_v1 = jtoc_s0;` (com agrupamento de literais) |
| `System.out.print` / `println` | `printf` (com formato adequado ao tipo) |
| `Math.sqrt` / `Math.pow` | `sqrtf` / `powf` (`<math.h>`) |
| Estruturas de controle (`if`, `while`, `for`, `switch`) | Tradução direta de blocos, laços e saltos |

## **Decisões Técnicas**

* **Foco no Essencial:** O projeto foca em criar uma base sólida para as fases de compilação (análise léxica, sintática e semântica), deixando de fora recursos complexos e "pesados" da Orientação a Objetos, como herança ou a diretiva `new`.


* **Ferramentas Clássicas:** Utilização do **Flex** para a geração do *scanner* léxico e do **Bison** para o *parser* sintático, integrados de forma automatizada em um *pipeline* de compilação em C.



---

## **Glossário de Tokens**

O analisador léxico reconhece os elementos principais do Java procedural. A lista abaixo descreve os *tokens* reconhecidos; isso não significa que todos sejam aceitos pelo parser. Por exemplo, classes, métodos, `do`, `continue`, `return`, `final`, `null` e tipos adicionais não fazem parte da gramática.

### 1. Palavras-Chave e Modificadores

* **Estrutura:** `public`, `class`, `static`, `void`, `final`.


* **Controle de Fluxo:** `if`, `else`, `switch`, `case`, `default`, `while`, `do`, `for`, `break`, `continue`, `return`.



### 2. Tipos de Dados Suportados

* **Tipos reconhecidos pelo scanner:** `boolean`, `byte`, `char`, `short`, `int`, `long`, `float`, `double` e `String`. A gramática aceita apenas `boolean`, `char`, `int`, `float` e `String`.



### 3. Literais e Identificadores

* **Identificadores:** Nomes de classes, métodos e variáveis (seguindo o padrão `[a-zA-Z_][a-zA-Z0-9_]*`).


* **Numéricos:** Inteiros decimais (`[0-9]+`) de 32 bits com sinal e ponto flutuante (`[0-9]+\.[0-9]+`). O sinal negativo é um operador; a magnitude `2147483648` só é aceita sob negação unária, formando `-2147483648`.


* **Texto:** Caracteres (`'c'`) e strings (`"texto"`), sem quebras de linha reais. Escapes aceitos: `\b`, `\t`, `\n`, `\f`, `\r`, `\"`, `\'` e `\\`. Outros escapes são erros léxicos; escapes octais e Unicode estão fora do subconjunto.


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

O subconjunto implementado tem os seguintes limites:

* **Tipos:** Inicializadores e atribuições simples aceitam tipos iguais e conversão de `int` para `float`. Atribuições compostas, incremento e decremento operam sobre `int` e `float`. As condições de `if`, `while` e `for` devem ser booleanas quando presentes. A inferência cobre literais, identificadores e operadores aritméticos, relacionais, lógicos e de igualdade nesses contextos.
* **Inicialização:** É verificada nos modos de geração de código (`--ir`, `--c` e `-o`). A análise reconhece condições booleanas constantes e comparações de expressões constantes inteiras, compartilhando a aritmética dos casos de `switch`. Condições com variáveis ou cálculos de ponto flutuante permanecem desconhecidas; a análise pode rejeitar programas válidos que dependam de provar essas condições. Nomes iguais em escopos diferentes são aceitos.
* **Funções e texto:** `Math.sqrt` e `Math.pow` recebem números e retornam `float`. Não há funções do usuário, chamadas gerais ou `return`. Não há concatenação de strings nem promoção numérica de `char`.

* **Limites do `for`:** Inicialização por declaração ou atribuição e atualização por atribuição, incremento ou decremento, com cláusulas opcionais. Listas separadas por vírgulas não estão implementadas.

* **Orientação a Objetos avançada:** Diretiva `new`, herança, polimorfismo e classes aninhadas.


* **Tratamento de Exceções:** Palavras-chave como `try`, `catch`, `finally`, `throw` e `throws`.


* **Recursos Modernos e Dinâmicos:** Generics, Lambdas, Streams, Reflection e Threads.



---

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação da documentação inicial | 22/09/2026 | [Henrique](https://github.com/henryqma) |
| 2.0 | Atualização do desenvolvimento para o P2 | 06/11/2026 | [Nicolas](https://github.com/nicolaszanin06) |
| 3.0 | Atualização para a entrega final | 27/11/2026 | [Henrique](https://github.com/henryqma) |

---