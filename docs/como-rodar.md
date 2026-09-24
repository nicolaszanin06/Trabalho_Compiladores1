# **Como Rodar**

Esta seção apresenta o passo a passo para configurar o ambiente, compilar o projeto e executar o analisador léxico e sintático.

## **Pré-requisitos**

Para compilar o projeto corretamente, é necessário ter as seguintes ferramentas instaladas no seu ambiente de desenvolvimento:

* `gcc` (Compilador C)
* `flex` (Gerador do analisador léxico)
* `bison` (Gerador do analisador sintático)
* `make` (Ferramenta de automação de compilação)

## **1. Clonar o Repositório**

Primeiro, faça o clone do repositório da equipe para a sua máquina local:

```bash
git clone https://github.com/nicolaszanin06/Trabalho_Compiladores1
cd Trabalho_Compiladores1

```

## **2. Compilar o Projeto**

O projeto utiliza um arquivo `Makefile` para organizar a compilação e os testes de forma automatizada. Na raiz do repositório, basta executar o seguinte comando:

```bash
make

```

Este comando irá processar os arquivos do Flex (`.l`) e do Bison (`.y`), gerando os códigos intermediários em C (`lex.yy.c`, `scanner.tab.c` e `scanner.tab.h`) e gerando o executável final chamado `scanner`.

## **3. Executar o Analisador**

Para analisar um arquivo-fonte em Java, execute o programa passando o arquivo como entrada através de redirecionamento no terminal:

```bash
./scanner < teste.java

```

Onde `teste.java` é o arquivo que contém o código a ser avaliado pelo compilador.

## **4. Execução Automatizada de Testes**

O projeto conta com casos executáveis da gramática atual localizados no diretório `testes/`. Se você estiver em um ambiente Windows com **PowerShell**, pode rodar a bateria de testes de forma automatizada executando o script fornecido na raiz do projeto:

```powershell
.\testes\executar.ps1

```

## **5. Limpar os Arquivos Gerados**

Se precisar limpar o seu diretório de trabalho, removendo os arquivos intermediários e binários gerados durante o processo de compilação, utilize o comando:

```bash
make clean

```

Este comando assegura que os seguintes arquivos sejam deletados:

* `lex.yy.c`
* `scanner.tab.c`
* `scanner.tab.h`
* `scanner`

---

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| --- | --- | --- | --- |
| 1.0 | Criação do guia de execução | 24/09/2026 | [Henrique](https://github.com/henryqma) |