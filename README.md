# ☕ Java Procedural - Analisador Léxico & Compilador

Projeto desenvolvido para a disciplina **FGA0003 - Compiladores 1** (UnB - Faculdade do Gama), implementando um analisador léxico (Scanner) com **Flex** e **Bison** para um subconjunto procedural da linguagem Java.

---

## 📌 Especificação dos Tokens Reconhecidos

### 1. Palavras-Chave e Modificadores

**Estrutura:**

* `public`
* `class`
* `static`
* `void`
* `final`

**Controle de Fluxo:**

* `if`
* `else`
* `switch`
* `case`
* `default`
* `while`
* `do`
* `for`
* `break`
* `continue`
* `return`

**Tipos de Dados:**

* `boolean`
* `byte`
* `char`
* `short`
* `int`
* `long`
* `float`
* `double`

### 2. Literais e Identificadores

* **Identificadores:** nomes de classes, métodos e variáveis (`[a-zA-Z_][a-zA-Z0-9_]*`)
* **Inteiros:** `[0-9]+`
* **Ponto Flutuante:** `[0-9]+\.[0-9]+`
* **Caracteres:** `'c'` (`CHAR_LITERAL`)
* **Strings:** `"texto"` (`STRING_LITERAL`)
* **Valores fixos:** `true`, `false`, `null`

### 3. Operadores

**Aritméticos:**

* `+`
* `-`
* `*`
* `/`
* `%`

**Relacionais:**

* `==`
* `!=`
* `<`
* `<=`
* `>`
* `>=`

**Lógicos:**

* `&&`
* `||`
* `!`

**Atribuição:**

* `=`

### 4. Delimitadores e Pontuação

* `;`
* `,`
* `.`
* `(`
* `)`
* `{`
* `}`
* `[`
* `]`

### 5. Tratamento de Espaços e Comentários

O analisador ignora:

* Espaços em branco
* Quebras de linha (`\n`)
* Tabulações (`\t`)
* Retorno de carro (`\r`)
* Comentários de linha única (`// ...`)
* Comentários de bloco (`/* ... */`)

---

## 🚫 Limitações de Escopo

O projeto **não suporta** os seguintes recursos da linguagem Java:

* Orientação a Objetos avançada (`new`, herança, polimorfismo e classes aninhadas)
* Tratamento de exceções (`try`, `catch`, `finally`, `throw`, `throws`)
* Generics
* Lambdas
* Streams
* Reflection
* Threads
* Outros recursos dinâmicos da JVM

O projeto tem como objetivo trabalhar com um **subconjunto procedural da linguagem Java**, com foco nos conceitos de análise léxica e compilação.

---

## 🛠️ Como Compilar e Executar

### Pré-requisitos

É necessário ter instalado:

* `gcc`
* `flex`
* `bison`
* `make`

### 1. Compilar o Projeto

Na raiz do projeto, execute:

```bash
make
```

O comando processará os arquivos `.l` e `.y` e realizará a compilação do executável `scanner`.

### 2. Executar com Arquivo de Teste

Para analisar um arquivo-fonte Java, utilize o redirecionamento da entrada padrão:

```bash
./scanner < teste.java
```

### 3. Limpar os Arquivos Gerados

Para remover os arquivos intermediários e o executável:

```bash
make clean
```

Os arquivos removidos incluem:

* `lex.yy.c`
* `scanner.tab.c`
* `scanner.tab.h`
* `scanner`

---

## 🧪 Exemplos de Teste

### Exemplo de Entrada Válida

Arquivo `teste.java`:

```java
public class Exemplo {
    public static void main() {
        int a = 10;
        float b = 20.5;
        if (a < b) {
            return;
        }
    }
}
```

### Saída Produzida pelo Scanner

```text
PUBLIC CLASS ID(Exemplo) LBRACE
PUBLIC STATIC VOID ID(main) LPAREN RPAREN LBRACE
INT ID(a) ASSIGN NUM(10) SEMI
FLOAT ID(b) ASSIGN FLOAT_LITERAL(20.5) SEMI
IF LPAREN ID(a) LT ID(b) RPAREN LBRACE
RETURN SEMI
RBRACE
RBRACE
RBRACE
```

---

## ❌ Tratamento de Erros

O analisador também realiza o tratamento de alguns erros léxicos.

### Caractere Inválido

Entrada:

```java
@
```

Saída esperada:

```text
Erro lexico na linha X: Caractere invalido '@'
```

### String Não Encerrada

Entrada:

```java
"teste
```

Saída esperada:

```text
Erro lexico na linha X: String nao encerrada
```

---

## 📂 Estrutura do Repositório

| Arquivo     | Descrição                                                       |
| ----------- | --------------------------------------------------------------- |
| `scanner.l` | Especificação das expressões regulares e regras léxicas do Flex |
| `scanner.y` | Definição dos tokens e gramática inicial no Bison               |
| `Makefile`  | Automação do processo de compilação e limpeza                   |
| `README.md` | Documentação, especificação e instruções de uso                 |

---

## 🎯 Objetivo do Projeto

O projeto tem como objetivo aplicar, de forma prática, conceitos fundamentais de **Compiladores**, especialmente:

* Análise léxica
* Expressões regulares
* Reconhecimento de tokens
* Tratamento de erros léxicos
* Integração entre **Flex** e **Bison**
* Geração e compilação de código em C
* Construção de uma gramática inicial para um subconjunto da linguagem Java

---

## 👨‍💻 Disciplina

**FGA0003 - Compiladores 1**
**Universidade de Brasília (UnB) - Faculdade do Gama (FGA)**
