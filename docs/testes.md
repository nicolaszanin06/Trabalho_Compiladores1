# Testes e validação

## Suíte automatizada

Na raiz do repositório, com Python 3, GCC, Make, Flex e Bison disponíveis, compile o executável e execute as suítes nesta ordem:

```bash
make -B -C src
python3 testes/test_analisador.py --regenerate
python3 testes/test_gerador_c.py
```

Se necessário, substitua `python3` por `python`. O build prepara `src/jtoc`, usado pela suíte do backend C. A suíte do analisador compila sua própria cópia e o observador de tokens (`lexer_driver.c`) em pastas temporárias; os programas C gerados pelos testes também são compilados em pastas temporárias.

A suíte é dividida em duas frentes principais:

* **`test_analisador.py` e `executor_ir.py`:** cobrem análise léxica e diagnósticos de tokens, precedência na AST, tabela de símbolos, escopos, compatibilidade de tipos, atribuições compostas, `for`, `switch` (validação de seletor, constantes e unicidade de `case`), contexto de `break`, verificação de inicialização antes da leitura e emissão/simulação de Código Intermediário (`--ir`).
* **`test_gerador_c.py`:** valida a geração de código C (`--c` e `-o`), compilando os arquivos gerados com `gcc -std=c11 -Wall -Wextra -Wpedantic` (com `-lm`) e executando os binários resultantes para comparar `stdout`, `stderr` e códigos de saída, além de testar flags da CLI e rejeição de variáveis não inicializadas.

Os testes de `break` verificam a rejeição fora de laços e `switch`, inclusive depois do fechamento dessas estruturas, e a aceitação em `while`, `for` e `switch` com condicionais, blocos e estruturas aninhadas.

O workflow de testes do GitHub Actions faz o build e executa as duas suítes a cada push e pull request. Apenas `test_analisador.py` recebe `--regenerate`, para regenerar Flex/Bison em sua pasta temporária; sem essa flag, essa suíte utiliza os arquivos C gerados versionados. `test_gerador_c.py` não aceita `--regenerate` e utiliza o executável preparado pelo build.

## Arquivos de entrada

Além dos casos definidos nas suítes Python, há 14 arquivos classificados pelo resultado esperado:

* `testes/validos/`: seis entradas aceitas, incluindo declarações, inicialização, condição com identificador booleano, blocos aninhados, comentários e entrada vazia.
* `testes/invalidos/`: oito entradas rejeitadas, cobrindo ponto e vírgula ausente, expressão ausente, texto solto, caractere inválido, string não encerrada, comentário não encerrado e literais com quebras de linha reais.

As entradas válidas retornam código 0 e imprimem sucesso. A AST e a tabela aparecem somente com `--debug`. Erros léxicos, sintáticos ou semânticos retornam código 1 e impedem sucesso, dumps e geração de código. Literais com quebra de linha podem gerar erros léxicos e sintáticos na mesma execução.

Para uma demonstração clara, use `declaracoes.java` e `ifs_aninhados.java` como válidos; `sem_ponto_e_virgula.java` e `expressao_ausente.java` como erros sintáticos; `caractere_invalido.java` e `string_nao_encerrada.java` como erros léxicos.

## Demonstração no WSL/Linux

A partir da raiz, compile e execute:

```bash
cd src
make -B
./jtoc --debug < ../testes/validos/declaracoes.java
./jtoc < ../testes/invalidos/expressao_ausente.java
./jtoc < ../testes/invalidos/caractere_invalido.java

# Erros semânticos: cada entrada retorna código 1
printf '%s\n' 'int x; int x;' | ./jtoc
printf '%s\n' 'x = 1;' | ./jtoc
printf '%s\n' 'if (true) { break; }' | ./jtoc
echo $? # código da última execução

# Geração de IR e geração/execução de código C
printf '%s\n' 'int a = 3; int b = 4; System.out.println(a + b);' | ./jtoc --ir
printf '%s\n' 'int a = 3; int b = 4; System.out.println(a + b);' | ./jtoc -o /tmp/saida.c
gcc -std=c11 -Wall -Wextra -Wpedantic /tmp/saida.c -o /tmp/saida -lm && /tmp/saida
```

Os três exemplos de erro semântico escrevem, respectivamente, `Variável já declarada 'x'`, `Variável não declarada 'x'` e `'break' fora de laço ou switch`, com o prefixo `Erro semântico:` e ponto final, em stderr. Eles deixam stdout vazio mesmo com `--debug`.

## Verificação no Windows e escopo coberto

No PowerShell, com GCC instalado no Windows, execute `./testes/executar.ps1` a partir da raiz. Esse verificador cobre os 14 arquivos com `--debug` (comparando stdout, stderr e código de saída), cinco casos de geração de código intermediário (`--ir`) e um teste ponta a ponta de geração de código C, compilação via GCC e execução. Alterações em `jtoc.l` ou `jtoc.y` exigem regenerar os arquivos C antes de usar o script PowerShell.

Todas as etapas previstas no escopo do compilador — análise léxica, sintática, semântica (incluindo compatibilidade de tipos, validação de `switch` e inicialização antes da leitura nos modos `--ir` e `--c`), geração de código intermediário e geração de código C compilável — são validadas continuamente pelas suítes automatizadas. O roteiro completo e a descrição de cada arquivo estão no [README de testes no repositório](https://github.com/nicolaszanin06/Trabalho_Compiladores1/blob/main/testes/README.md).

## Cobertura numérica, CLI e memória

A suíte do analisador cobre os limites de inteiros em expressões e `case`, o mínimo `-2147483648`, overflow de literais `float`, underflow de não zero para zero, zero e subnormais aceitos. Exercita os modos padrão, IR e C na validação de `float`. Condições constantes inteiras e booleanas são confrontadas com a análise de inicialização, incluindo curto-circuito e saídas por `break`.

O driver `alocacao_driver.c` injeta falhas de alocação em `texto_expressao` e verifica a liberação de resultados parciais. Isso não é uma prova geral de ausência de vazamentos de todo o compilador. `lexer_driver.c` observa tokens e linhas sem executar o parser.

A suíte C inclui overflow com wrap-around, divisão e resto especiais do mínimo inteiro, diagnóstico de divisão por zero em execução, limites e subnormais de `float`, strings UTF-8 e identidade de literais, escopos com nomes conflitantes em C, matemática aninhada, laços e `switch` com fall-through. Verifica também stdout separado de debug, arquivo existente preservado, ordem das flags, repetição do mesmo modo, exclusão IR/C, `-o` ausente ou repetido e ausência de destino após erro semântico. Os testes selecionados que exigem ausência de warnings não implicam que todo programa válido seja compilado sem avisos (variáveis não utilizadas podem gerar warnings).

`test_analisador.py` compila seus próprios executáveis temporários com `CC` (padrão `gcc`), independentemente de `src/jtoc`. A suíte C usa `JTOC` para selecionar outro executável de tradução e `CC` para compilar o C. No Windows, o padrão é `src/jtoc.exe`; o script `executar.ps1` compila e remove seu próprio verificador, sem preparar esse executável para a suíte Python C. Veja [Como rodar](como-rodar.md).

## Reprodução e alcance da validação

`test_analisador.py` possui limites de tempo para compilação e chamadas aos drivers; `test_gerador_c.py` limita a tradução a 10 segundos, a compilação a 30 e a execução a 5. O executor auxiliar da IR limita passos e verifica pilha vazia ao terminar. Sua impressão de `float` usa a representação Python, distinta das seis casas do backend C.

As suítes registram regressões específicas; não provam equivalência com Java completo ou cobertura exaustiva de ponto flutuante, domínio matemático e falhas de E/S. O workflow de testes executa as duas suítes em push/PR. O workflow da documentação publica via MkDocs apenas nos pushes às branches configuradas (`main` e `master`); build local não publica nada.

## Registro da auditoria da documentação — 27/11/2026

A revisão confrontou as páginas com as fontes e os testes locais do compilador final:

| Fontes conferidas | Contrato documentado |
| --- | --- |
| `src/jtoc.l` | Lexemas, escapes, comentários, tokens reservados e canais dos erros léxicos |
| `src/jtoc.y` e `src/Makefile` | Gramática, precedência, CLI, integração, publicação da saída e regeneração paralela |
| `src/ast.*` e `src/tabela_simbolos.*` | Modelo filho-irmão, metadados, escopos, tipos, constantes, limites numéricos e inicialização por caminhos |
| `src/gerador_intermediario.*` e `testes/executor_ir.py` | Instruções de pilha, conversões, nomes, saltos e limites do executor auxiliar |
| `src/gerador_c.*` | C11 direto da AST, auxiliares inteiros, curto-circuito, strings, impressão, fall-through e limitações do alvo |
| `testes/test_analisador.py`, `testes/test_gerador_c.py` e `testes/executar.ps1` | Dependências reais, executáveis temporários, cobertura, timeouts, configuração e reprodução |

Na validação desta revisão, o build forçado e paralelo com Flex/Bison/GCC passou em cópia temporária, seguido de **71 testes do analisador/IR e 30 do backend C**, todos aprovados. O uso de cópias temporárias preservou os fontes e os testes do repositório. O script PowerShell foi inspecionado; sua execução nativa não fez parte dessa rodada Linux/WSL. Esses resultados registram esta revisão e não substituem novas execuções após futuras mudanças.

## **Histórico de Versões**

| VERSÃO | DESCRIÇÃO | DATA | AUTOR |
| ------ | --------- | ---- | ----- |
| 1.0 | Criação da documentação inicial | 22/09/2026 | [Henrique](https://github.com/henryqma) |
| 2.0 | Atualização do desenvolvimento para o P2 | 06/11/2026 | [Nicolas](https://github.com/nicolaszanin06) |
| 3.0 | Atualização da suíte de testes para a entrega final | 27/11/2026 | [Henrique](https://github.com/henryqma) |
