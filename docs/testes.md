# **Testes e Validação**

A validação do compilador é realizada através de um conjunto de arquivos de teste organizados no diretório `testes/`. O objetivo desta etapa é garantir que os analisadores léxico e sintático se comportem conforme o esperado, tanto ao compilar um código correto quanto ao identificar falhas no código-fonte.

## **Estrutura do Diretório de Testes**

Para manter a organização e facilitar a verificação dos casos de uso, a equipe dividiu os cenários de teste em duas categorias principais dentro do repositório:

### 1. Testes Válidos (`testes/validos/`)

Esta pasta contém arquivos com códigos escritos no subconjunto de Java suportado pelo compilador. Eles devem ser processados até o fim sem gerar nenhum erro sintático ou léxico. Os arquivos implementados são:

* **`comentarios.java`**: Valida se o analisador léxico ignora corretamente os blocos de comentários sem gerar *tokens*.
* **`declaracoes.java`**: Verifica a sintaxe de declaração de variáveis (como `int`, `float` e `char`) seguidas de ponto e vírgula.
* **`ifs_aninhados.java`**: Testa o reconhecimento estrutural de blocos condicionais dentro de outros blocos.
* **`vazio.java`**: Garante que o *parser* consiga processar adequadamente um arquivo sem nenhum comando (fim de arquivo imediato).

### 2. Testes Inválidos (`testes/invalidos/`)

Esta pasta abriga códigos propositalmente incorretos. O objetivo é acionar os mecanismos de tratamento de erro construídos no Flex e no Bison, garantindo que o compilador não quebre (crash) e exiba a mensagem de erro na linha correta. Os cenários cobrem:

* **Erros Léxicos:**
* `caractere_invalido.java`: Insere símbolos que não pertencem à linguagem.
* `string_nao_encerrada.java`: Abre aspas duplas sem as fechar antes da quebra de linha.


* **Erros Sintáticos:**
* `sem_ponto_e_virgula.java`: Omissão do terminador de instrução `;`.
* `atribuicao_na_declaracao.java`: Tenta atribuir um valor no momento da declaração (ex: `int contador = 10;`), o que é intencionalmente testado para falhar caso a gramática atual ainda não suporte essa união de regras.
* `condicao_nao_suportada.java`: Utiliza uma expressão dentro de um `if` que a gramática ainda não consegue avaliar.



## **Automação dos Testes**

Para evitar a execução manual arquivo por arquivo, o projeto conta com um *script* de automação desenvolvido para ambientes Windows.

O arquivo **`executar.ps1`** localizado na pasta `testes/` é um script em PowerShell que itera automaticamente sobre os arquivos de teste, passa cada um deles para o executável do `scanner` e imprime os resultados da compilação na tela do desenvolvedor.

---

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| --- | --- | --- | --- |
| 1.0 | Criação da documentação de Testes e Validação | 24/09/2026 | [Henrique](https://github.com/henryqma) |