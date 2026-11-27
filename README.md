# Java Procedural → C — Compiladores 1

Projeto final de **Compiladores 1 (FGA0003 — UnB), Grupo 7**. O `jtoc` traduz um subconjunto procedural de Java para C11 usando Flex, Bison, AST e análise semântica. Também oferece uma saída intermediária de pilha para inspeção e testes.

**[Documentação publicada no GitHub Pages](https://nicolaszanin06.github.io/Trabalho_Compiladores1/)**

## Compilar e usar

No WSL/Ubuntu, instale GCC, GNU Make 4.3 ou superior, Flex, Bison e libfl. Execute na raiz:

```bash
sudo apt update
sudo apt install build-essential flex bison libfl-dev
make -B -C src
./src/jtoc < testes/validos/declaracoes.java
./src/jtoc --debug testes/validos/declaracoes.java
./src/jtoc --ir testes/validos/declaracoes.java
./src/jtoc --c testes/validos/declaracoes.java
./src/jtoc -o programa.c testes/validos/declaracoes.java
gcc -std=c11 -Wall -Wextra -Wpedantic programa.c -o programa -lm
./programa
```

`make -B` regenera Flex/Bison mesmo com arquivos gerados versionados; `make -B -j4 -C src` também é suportado. O executável não é versionado. `make -C src clean` remove o executável e os arquivos gerados.

A entrada contém comandos diretamente, sem classe nem método `main`:

```java
int soma = 0;
for (int i = 0; i < 5; i++) {
    soma += i;
}
System.out.println(soma);
```

| Modo | Resultado |
| --- | --- |
| Sem opção de geração | Valida léxico, sintaxe, escopos e tipos; imprime a mensagem de sucesso. |
| `--ir` | Verifica também inicialização antes da leitura e emite IR em stdout. |
| `--c` | Verifica também inicialização e emite um programa C11 em stdout. |
| `-o arquivo.c` | Seleciona geração C e grava em um arquivo novo; recusa sobrescrever destinos existentes. |
| `--debug` | Mostra AST e tabela: stdout no modo padrão, stderr nos modos de geração. |

Uso: `jtoc [--debug] [--ir | --c] [-o arquivo.c] [entrada.java]`. Sem arquivo, lê stdin. `--c` e `-o` podem aparecer em qualquer ordem; repetir `--c` ou `--ir` é permitido. Misturar IR com C ou `-o`, repetir `-o`, omitir seu argumento, passar opções desconhecidas ou vários arquivos causa erro. Não há opção `--help` implementada. O compilador não executa o programa de entrada.

O retorno é `0` em sucesso e `1` em erro. No modo padrão, erros léxicos vão para stdout; nos modos de geração, vão para stderr. Erros sintáticos e semânticos vão para stderr. A mensagem de sucesso, AST e tabela só aparecem após validação bem-sucedida. A checagem de inicialização ocorre **somente nos modos de geração**, inclusive quando selecionados por `-o`.

## Linguagem e tradução

São aceitos `int`, `float`, `char`, `boolean`, `String`, declarações, `=`, `+=`, `-=`, `*=`, `/=`, `++`/`--` como comandos, blocos, `if`/`else`, `while`, `for`, `switch` tradicional com fall-through, `break`, impressão, expressões e `Math.sqrt`/`Math.pow`. Corpos de condicionais e laços exigem chaves. Não há listas de declarações ou atualizações separadas por vírgula.

Inteiros decimais usam 32 bits com sinal; `-2147483648` tem tratamento especial. Literais `float` exigem dígitos antes e depois do ponto, sem expoente ou sufixo; overflow e valores não nulos arredondados para zero são rejeitados, mas subnormais representáveis são aceitos. Comentários de linha e bloco são ignorados; os escapes permitidos estão no [Léxico](docs/analisador-lexico.md).

O backend C percorre a AST validada diretamente, preserva wrap-around inteiro, curto-circuito, identidade de literais `String`, nomes únicos por símbolo e fall-through. Divisão/resto inteiro por zero termina o programa gerado com diagnóstico. A impressão de `float` usa seis casas decimais; `char` no alvo C representa um byte. Use `-lm` para `Math.sqrt`, `Math.pow` e resto de `float` (pode ser mantido nos demais exemplos).

Não há otimização, orientação a objetos, funções do usuário, `return`, arrays, leitura de entrada ou concatenação. Tokens reconhecidos pelo scanner, como `class`, `double`, `continue` e `final`, não ampliam a gramática. Consulte [Escopo](docs/escopo.md), [Arquitetura e decisões](docs/arquitetura-decisoes.md), [IR](docs/codigo-intermediario.md) e [Geração C](docs/geracao-c.md).

## Testes

Após compilar, execute na raiz:

```bash
python3 testes/test_analisador.py --regenerate
python3 testes/test_gerador_c.py
```

A suíte do analisador compila cópias temporárias; `--regenerate` requer Flex/Bison e pode ser omitido para usar os arquivos gerados versionados. A suíte C usa `src/jtoc` já compilado, traduz, compila com GCC e compara a execução. Python não requer pacotes adicionais para essas suítes.

No PowerShell com GCC nativo, `python testes/test_analisador.py` executa a suíte do analisador; `.\testes\executar.ps1` compila um verificador próprio e cobre 14 entradas, cinco casos IR e um caso C ponta a ponta. Veja [Como rodar](docs/como-rodar.md) e [Testes e validação](docs/testes.md).

Para visualizar a documentação localmente:

```bash
python3 -m pip install mkdocs-material
python3 -m mkdocs build --strict
python3 -m mkdocs serve
```

## Equipe e organização

| Integrante | Frente liderada, com apoio mútuo |
| --- | --- |
| [Vinicius Gavaldao](https://github.com/ViniGvdo) e [Warlley Medeiros](https://github.com/warlleymedeiros) | Análises léxica, sintática e início da semântica |
| [Nicolas Zanin](https://github.com/nicolaszanin06) | Testes automatizados e consolidação da semântica |
| [Henrique Alencar](https://github.com/henryqma) | Documentação e geração final de C |
| [Tiago Vieira](https://github.com/TiagoVieira-596) | Geração de código intermediário |

“Todos fazem tudo”: aulas e encontros presenciais serviram à discussão conjunta do escopo, das regras e dos problemas. As Sprints 0–13 foram impulsos de implementação, sem duração fixa rígida. O histórico está em [Organização, equipe e sprints](docs/organizacao-sprints.md).

O [planejamento](docs/planejamento.md) registra a evolução entre os pontos de controle. As [decisões de arquitetura](docs/arquitetura-decisoes.md) descrevem os problemas técnicos e as soluções adotadas em cada etapa. Esses documentos reúnem as informações pedidas na [proposta de trabalho da disciplina](https://github.com/sergioaafreitas/COMP1/blob/main/semana%2001/docs/Trabalho%20de%20Compiladores.md).

## Estrutura

| Caminho | Responsabilidade |
| --- | --- |
| `src/jtoc.l`, `src/jtoc.y` | Scanner, gramática, CLI e integração |
| `src/ast.*`, `src/tabela_simbolos.*` | AST filho-irmão, símbolos, tipagem e fluxo de inicialização |
| `src/gerador_intermediario.*`, `src/gerador_c.*` | Saídas alternativas: IR de pilha e C11 |
| `src/Makefile` | Regeneração, compilação e limpeza |
| `testes/` | Entradas, suítes, drivers e executor auxiliar da IR |
| `docs/`, `mkdocs.yml` | Documentação e navegação do site |
