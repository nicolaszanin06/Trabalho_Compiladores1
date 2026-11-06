"""Testes sem dependencias externas de Python. Execute com --regenerate no CI."""
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest


TESTES = Path(__file__).resolve().parent
SRC = TESTES.parent / "src"
SUCESSO = "Analise concluida com sucesso! Nenhum erro sintatico encontrado.\n"
REGENERAR = "--regenerate" in sys.argv
if REGENERAR:
    sys.argv.remove("--regenerate")


class AnalisadorTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporario = tempfile.TemporaryDirectory(prefix="compiladores-testes-")
        cls.addClassCleanup(cls.temporario.cleanup)
        pasta = Path(cls.temporario.name)
        for nome in ("jtoc.l", "jtoc.y", "jtoc.tab.c", "jtoc.tab.h", "lex.yy.c",
                     "ast.c", "ast.h", "tabela_simbolos.c", "tabela_simbolos.h"):
            shutil.copy2(SRC / nome, pasta / nome)

        def compilar(*comando):
            resultado = subprocess.run(comando, cwd=pasta, capture_output=True,
                                       text=True, timeout=60)
            if resultado.returncode:
                raise RuntimeError(f"Falha em {comando}:\n{resultado.stdout}{resultado.stderr}")

        if REGENERAR:
            compilar("bison", "-d", "jtoc.y")
            compilar("flex", "jtoc.l")
        sufixo = ".exe" if os.name == "nt" else ""
        cls.parser = pasta / ("parser" + sufixo)
        cls.lexer = pasta / ("lexer" + sufixo)
        cc = os.environ.get("CC", "gcc")
        compilar(cc, "-o", str(cls.parser), "jtoc.tab.c", "lex.yy.c", "ast.c",
                 "tabela_simbolos.c")
        compilar(cc, "-I", str(pasta), "-o", str(cls.lexer),
                 str(TESTES / "lexer_driver.c"), "lex.yy.c")

    def executar(self, executavel, entrada="", argumentos=()):
        return subprocess.run([str(executavel), *map(str, argumentos)], input=entrada,
                              capture_output=True, text=True, encoding="utf-8", timeout=5)

    def executar_parser(self, entrada="", argumentos=(), debug=True):
        if debug:
            argumentos = ("--debug", *argumentos)
        return self.executar(self.parser, entrada, argumentos)

    def conferir_lexer(self, entrada, esperado):
        resultado = self.executar(self.lexer, entrada)
        self.assertEqual(resultado.returncode, 0)
        self.assertEqual(resultado.stderr, "")
        self.assertEqual(resultado.stdout, esperado)

    def test_palavras_reservadas(self):
        palavras = "public class static void boolean byte char double float int long short if else switch case default while do for break continue return final true false null".split()
        tokens = "PUBLIC CLASS STATIC VOID BOOLEAN BYTE CHAR DOUBLE FLOAT INT LONG SHORT IF ELSE SWITCH CASE DEFAULT WHILE DO FOR BREAK CONTINUE RETURN FINAL TOKEN_TRUE TOKEN_FALSE NULL_LITERAL".split()
        self.conferir_lexer(" ".join(palavras), "".join(
            f"{token}|{palavra}|1\n" for palavra, token in zip(palavras, tokens)))

    def test_identificadores_e_prefixos_de_palavras_reservadas(self):
        nomes = ["x", "_", "_valor2", "contador123", "intValue", "ifelse", "trueValue", "Int"]
        self.conferir_lexer(" ".join(nomes), "".join(
            f"IDENTIFIER|{nome}|1\n" for nome in nomes))

    def test_operadores_e_delimitadores(self):
        simbolos = "+ - * / % ++ -- == != < <= > >= && || ! = { } ( ) [ ] ; ,".split()
        tokens = "PLUS MINUS MULT DIV MOD INC DEC EQ NEQ LT LE GT GE AND OR NOT ASSIGN LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET SEMI COMMA".split()
        self.conferir_lexer(" ".join(simbolos), "".join(
            f"{token}|{simbolo}|1\n" for simbolo, token in zip(simbolos, tokens)))

    def test_operadores_colados_usam_maior_correspondencia(self):
        self.conferir_lexer("a++<=b!=c--", "IDENTIFIER|a|1\nINC|++|1\nLE|<=|1\nIDENTIFIER|b|1\nNEQ|!=|1\nIDENTIFIER|c|1\nDEC|--|1\n")

    def test_literais_e_lexema_inteiro_preservado(self):
        self.conferir_lexer('0 007 123 12.50 \'a\' \'\\n\' "ola" "a\\\"b"',
                            'INT_LITERAL|0|1|0\nINT_LITERAL|007|1|007\nINT_LITERAL|123|1|123\nFLOAT_LITERAL|12.50|1\nCHAR_LITERAL|\'a\'|1\nCHAR_LITERAL|\'\\n\'|1\nSTRING_LITERAL|"ola"|1\nSTRING_LITERAL|"a\\\"b"|1\n')

    def test_comentarios_ignoram_tokens_e_preservam_linhas(self):
        self.conferir_lexer('// if @\n/* int\n @ */\n\tint x;',
                            "INT|int|4\nIDENTIFIER|x|4\nSEMI|;|4\n")

    def test_comentario_termina_no_primeiro_fechamento(self):
        self.conferir_lexer("/* a **/int x;/* b */", "INT|int|1\nIDENTIFIER|x|1\nSEMI|;|1\n")

    def test_barra_sem_comentario_e_divisao(self):
        self.conferir_lexer("a/b", "IDENTIFIER|a|1\nDIV|/|1\nIDENTIFIER|b|1\n")

    def test_espacos_crlf_e_linha_do_token(self):
        self.conferir_lexer(" \t\r\nint\r\nx;", "INT|int|2\nIDENTIFIER|x|3\nSEMI|;|3\n")

    def test_erro_lexico_informa_linha_e_continua(self):
        self.conferir_lexer("\n@ int x;", "Erro lexico na linha 2: Caractere invalido '@'\nINT|int|2\nIDENTIFIER|x|2\nSEMI|;|2\n")

    def test_string_nao_encerrada_no_fim_do_arquivo(self):
        self.conferir_lexer('"texto', "Erro lexico na linha 1: String nao encerrada\n")

    def test_arquivos_validos(self):
        for arquivo in sorted((TESTES / "validos").glob("*.java")):
            with self.subTest(arquivo=arquivo.name):
                resultado = self.executar_parser(argumentos=[arquivo])
                self.conferir_sucesso(resultado)

    def conferir_sucesso(self, resultado):
        self.assertEqual(resultado.returncode, 0)
        self.assertEqual(resultado.stderr, "")
        self.assertTrue(resultado.stdout.startswith(SUCESSO + "AST:\nBloco\n"), resultado.stdout)
        self.assertIn("\nTabela de simbolos (valor de inicializacao):\n", resultado.stdout)

    def test_arquivos_com_erros_sintaticos(self):
        for nome in ("sem_ponto_e_virgula", "expressao_ausente", "texto_solto"):
            with self.subTest(arquivo=nome):
                resultado = self.executar_parser(argumentos=[TESTES / "invalidos" / (nome + ".java")])
                self.assertEqual(resultado.returncode, 1)
                self.assertEqual(resultado.stdout, "")
                self.assertRegex(resultado.stderr, r"\AErro sintatico na linha \d+: syntax error perto de '.*'\n\Z")

    def test_erros_lexicos_rejeitam_entrada_sem_imprimir_sucesso(self):
        for nome, mensagem in (("caractere_invalido", "Caractere invalido '@'"),
                               ("string_nao_encerrada", "String nao encerrada"),
                               ("comentario_nao_encerrado", "Comentario nao encerrado")):
            with self.subTest(arquivo=nome):
                arquivo = TESTES / "invalidos" / (nome + ".java")
                resultado = self.executar_parser(argumentos=[arquivo])
                self.assertEqual(resultado.returncode, 1)
                self.assertEqual(resultado.stderr, "")
                self.assertEqual(resultado.stdout, f"Erro lexico na linha 1: {mensagem}\n")

    def test_blocos_vazios_e_declaracoes_sem_espacos(self):
        resultado = self.executar_parser("if(true){}if(false){int x;float y;char z;}")
        self.conferir_sucesso(resultado)

    def test_erros_sintaticos_com_linha_e_token_exatos(self):
        casos = [("int x\nfloat y;", 2, "float"),
                 ("if (true) {\nint x;", 2, ""),
                 ("int x;\n}", 2, "}"),
                 ("if (true) {} else", 1, ""),
                 ("double x;", 1, "double")]
        for entrada, linha, token in casos:
            with self.subTest(entrada=entrada):
                resultado = self.executar_parser(entrada)
                self.assertEqual(resultado.returncode, 1)
                self.assertEqual(resultado.stdout, "")
                self.assertEqual(resultado.stderr, f"Erro sintatico na linha {linha}: syntax error perto de '{token}'\n")

    def test_tokens_de_string_impressao_e_matematica(self):
        self.conferir_lexer("String System.out.print System.out.println Math.sqrt Math.pow",
                            "STRING_TYPE|String|1\nPRINT|System.out.print|1\nPRINTLN|System.out.println|1\nMATH_SQRT|Math.sqrt|1\nMATH_POW|Math.pow|1\n")

    def test_literais_multilinha_invalidos(self):
        casos = [
            ("string_multilinha", "Erro lexico na linha 1: String nao encerrada\n",
             "Erro sintatico na linha 2: syntax error perto de 'linha'\n"),
            ("char_multilinha", "Erro lexico na linha 1: Caractere invalido '''\nErro lexico na linha 2: Caractere invalido '''\n",
             "Erro sintatico na linha 2: syntax error perto de ';'\n"),
        ]
        for nome, saida, erro in casos:
            with self.subTest(arquivo=nome):
                resultado = self.executar_parser(argumentos=[TESTES / "invalidos" / (nome + ".java")])
                self.assertEqual((resultado.returncode, resultado.stdout, resultado.stderr), (1, saida, erro))

    def test_ast_respeita_precedencia_e_parenteses(self):
        for expressao, arvore in [
            ("1 + 2 * 3", "    +\n      1\n      *\n        2\n        3\n"),
            ("(1 + 2) * 3", "    *\n      +\n        1\n        2\n      3\n"),
        ]:
            with self.subTest(expressao=expressao):
                resultado = self.executar_parser(f"int x = {expressao};")
                self.conferir_sucesso(resultado)
                ast = resultado.stdout.split("AST:\n", 1)[1].split("\nTabela de simbolos", 1)[0]
                self.assertEqual(ast, "Bloco\n  Declaracao\n    int\n    x\n" + arvore)

    def test_tabela_de_simbolos_preserva_inicializacao_e_escopos(self):
        resultado = self.executar_parser(
            "int x = 1 + 2; x = 9; if (true) { String x = \"local\"; } else { char c; } while(false) { float y = 2.5; }")
        self.conferir_sucesso(resultado)
        tabela = resultado.stdout.split("Tabela de simbolos (valor de inicializacao):\n", 1)[1]
        linhas = [[coluna.strip() for coluna in linha.split("|")] for linha in tabela.splitlines()]
        self.assertEqual(linhas, [
            ["Simbolo", "Tipo", "Valor", "Escopo"],
            ["x", "int", "(1 + 2)", "global"],
            ["x", "String", '"local"', "local"],
            ["c", "char", "nao inicializado", "local"],
            ["y", "float", "2.5", "local"],
        ])

    def test_comandos_e_funcoes_da_gramatica_atual(self):
        resultado = self.executar_parser(
            "int x = 0; boolean b = !false && true; while (x < 3) { System.out.println(Math.sqrt(x)); x++; } if (b) { System.out.print(Math.pow(2, 3)); } else { x--; }")
        self.conferir_sucesso(resultado)
        for comando in ("While", "Println", "Print", "Math.sqrt", "Math.pow", "If", "++", "--"):
            self.assertIn(comando, resultado.stdout)

    def test_arquivo_inexistente(self):
        resultado = self.executar_parser(argumentos=[Path(self.temporario.name) / "inexistente.java"])
        self.assertEqual(resultado.returncode, 1)
        self.assertEqual(resultado.stdout, "")
        self.assertIn("Erro ao abrir o arquivo", resultado.stderr)

    def test_tokens_de_atribuicao_composta_e_dois_pontos(self):
        self.conferir_lexer("x+=1;y-=2;z*=3;w/=4;case 1:",
            "IDENTIFIER|x|1\nADD_ASSIGN|+=|1\nINT_LITERAL|1|1|1\nSEMI|;|1\n"
            "IDENTIFIER|y|1\nSUB_ASSIGN|-=|1\nINT_LITERAL|2|1|2\nSEMI|;|1\n"
            "IDENTIFIER|z|1\nMUL_ASSIGN|*=|1\nINT_LITERAL|3|1|3\nSEMI|;|1\n"
            "IDENTIFIER|w|1\nDIV_ASSIGN|/=|1\nINT_LITERAL|4|1|4\nSEMI|;|1\n"
            "CASE|case|1\nINT_LITERAL|1|1|1\nCOLON|:|1\n")

    def test_ast_das_atribuicoes_compostas(self):
        for operador in ("+=", "-=", "*=", "/="):
            with self.subTest(operador=operador):
                resultado = self.executar_parser(f"int x = 1; x {operador} 2 + 3;")
                self.conferir_sucesso(resultado)
                self.assertIn(f"  {operador}\n    x\n    +\n      2\n      3\n", resultado.stdout)

    def test_ast_do_for_e_declaracoes_na_tabela(self):
        resultado = self.executar_parser(
            "for (int i = 0; i < 10; i++) { int dobro = i * 2; }")
        self.conferir_sucesso(resultado)
        ast = resultado.stdout.split("AST:\n", 1)[1].split("\nTabela de simbolos", 1)[0]
        self.assertEqual(ast, "Bloco\n  For\n    Declaracao\n      int\n      i\n      0\n"
            "    <\n      i\n      10\n    ++\n      i\n    Bloco\n"
            "      Declaracao\n        int\n        dobro\n        *\n          i\n          2\n")
        tabela = resultado.stdout.split("Tabela de simbolos", 1)[1]
        for nome in ("i", "dobro"):
            self.assertRegex(tabela, rf"(?m)^{nome}\s+\| int\s+\| .*\| local$")

    def test_for_com_clausulas_opcionais_e_atualizacoes(self):
        for cabecalho in (";;", "int i = 0;;", ";true;", "int i = 3;i > 0;i--",
                          "int i = 0;i < 3;i += 1", "i = 0;i < 3;i = i + 1"):
            with self.subTest(cabecalho=cabecalho):
                resultado = self.executar_parser(f"int i; for ({cabecalho}) {{}}")
                self.conferir_sucesso(resultado)
                self.assertIn("  For\n", resultado.stdout)
        resultado = self.executar_parser("for (;;) {}")
        self.assertIn("  For\n    Vazio\n    Vazio\n    Vazio\n    Bloco\n", resultado.stdout)

    def test_switch_com_cases_agrupados_default_e_break(self):
        resultado = self.executar_parser(
            "int x = 1; switch (x) { case 0: case 1: x += 2; break; "
            "default: int outro = 3; System.out.println(outro); }")
        self.conferir_sucesso(resultado)
        self.assertIn("  Switch\n    x\n    Casos\n      Case\n        0\n        Bloco\n"
                      "      Case\n        1\n        Bloco\n          +=\n", resultado.stdout)
        self.assertIn("          Break\n      Default\n        Bloco\n", resultado.stdout)
        self.assertRegex(resultado.stdout, r"(?m)^outro\s+\| int\s+\| 3\s+\| local$")

    def test_switch_vazio_e_estruturas_aninhadas(self):
        for entrada in ("switch (1) {}", "switch (1) { default: case 1: break; }",
                        "for (int i = 0; i < 2; i++) { switch (i) { case 1: "
                        "{ int n = i; } while (false) { break; } default: break; } }"):
            with self.subTest(entrada=entrada):
                self.conferir_sucesso(self.executar_parser(entrada))

    def test_rejeita_novas_estruturas_malformadas(self):
        entradas = ["int x; x +=;", "int x += 1;", "for (int i = 0 i < 2; i++) {}",
                    "for (int i = 0; i < 2; i++;) {}", "for (;;) int x;",
                    "switch (1) { case 1 break; }", "switch (1) { default break; }",
                    "case 1: int x;", "switch (1) { int x; }", "break"]
        for entrada in entradas:
            with self.subTest(entrada=entrada):
                resultado = self.executar_parser(entrada)
                self.assertEqual(resultado.returncode, 1)
                self.assertIn("Erro sintatico", resultado.stderr)
                self.assertNotIn(SUCESSO, resultado.stdout)

    def conferir_erro_semantico(self, entrada, mensagem, nome):
        resultado = self.executar_parser(entrada)
        self.assertEqual(resultado.returncode, 1)
        self.assertEqual(resultado.stdout, "")
        self.assertIn(f"Erro semântico: {mensagem} '{nome}'.\n", resultado.stderr)
        return resultado

    def test_rejeita_declaracoes_repetidas_no_mesmo_escopo(self):
        entradas = ["int x; float x;", "if (true) { int x; int x; }",
                    "for (;;) { int x; int x; }",
                    "switch (1) { case 1: int x; break; default: int x; }"]
        for entrada in entradas:
            with self.subTest(entrada=entrada):
                resultado = self.conferir_erro_semantico(entrada, "Variável já declarada", "x")
                self.assertEqual(resultado.stderr.count("Variável já declarada"), 1)

    def test_rejeita_atribuicao_e_incremento_sem_declaracao(self):
        for comando in ("x = 1;", "x += 1;", "x -= 1;", "x *= 1;", "x /= 1;", "x++;", "x--;"):
            with self.subTest(comando=comando):
                self.conferir_erro_semantico(comando, "Variável não declarada", "x")

    def test_rejeita_variavel_ausente_em_expressoes(self):
        entradas = ["int y = x + 1;", "int y; y = x;", "int y; y += x;",
                    "System.out.println(x);", "System.out.print(Math.sqrt(x));",
                    "int y = Math.pow(2, x);", "if (x) {}", "while (x < 3) {}",
                    "for (;x < 3;) {}", "switch (x) {}", "switch (1) { case x: break; }"]
        for entrada in entradas:
            with self.subTest(entrada=entrada):
                self.conferir_erro_semantico(entrada, "Variável não declarada", "x")

    def test_rejeita_uso_antes_da_declaracao(self):
        for entrada in ("x = 1; int x;", "int y = x; int x;",
                        "if (true) { System.out.println(x); } int x;"):
            with self.subTest(entrada=entrada):
                self.conferir_erro_semantico(entrada, "Variável não declarada", "x")

    def test_escopos_irmaos_nao_compartilham_variaveis(self):
        entradas = ["if (true) { int x; } x = 1;",
                    "if (true) { int x; } else { x = 1; }",
                    "if (true) { int x; } while (false) { x++; }",
                    "{ int x; } { System.out.println(x); }"]
        for entrada in entradas:
            with self.subTest(entrada=entrada):
                self.conferir_erro_semantico(entrada, "Variável não declarada", "x")

    def test_aceita_busca_em_escopo_externo_e_nomes_em_blocos_distintos(self):
        entradas = ["int x; if (true) { x = 1; while (false) { x++; } } x = 2;",
                    "if (true) { int x; } else { int x; } { int x; } int x;",
                    "int x = 1; { int x = 2; System.out.println(x); } x++;",
                    "int x; for (int i = 0; i < 2; i++) { x += i; }"]
        for entrada in entradas:
            with self.subTest(entrada=entrada):
                self.conferir_sucesso(self.executar_parser(entrada))

    def test_escopo_do_for_e_limites_da_visibilidade(self):
        entradas = [("for (int i = 0; i < 2; i++) {} i = 1;", "i"),
                    ("for (i = 0; i < 2; i++) {}", "i"),
                    ("for (int i = 0; i < 2; x++) {}", "x"),
                    ("for (;x < 2;) { int x; }", "x"),
                    ("for (;;x++) { int x; }", "x"),
                    ("for (;;) { int x; } System.out.print(x);", "x")]
        for entrada, nome in entradas:
            with self.subTest(entrada=entrada):
                self.conferir_erro_semantico(entrada, "Variável não declarada", nome)
        self.conferir_sucesso(self.executar_parser(
            "for (int i = 0; i < 2; i++) {} for (int i = 0; i < 2; i++) {} int i;"))

    def test_switch_compartilha_escopo_entre_cases_e_respeita_chaves(self):
        self.conferir_sucesso(self.executar_parser(
            "switch (1) { case 1: int x; case 2: x = 2; default: x++; }"))
        self.conferir_sucesso(self.executar_parser(
            "switch (1) { case 1: { int x; } break; default: { int x; } }"))
        for entrada in ("switch (1) { case 1: int x; } x = 2;",
                        "switch (1) { case 1: { int x; } default: x = 1; }"):
            with self.subTest(entrada=entrada):
                self.conferir_erro_semantico(entrada, "Variável não declarada", "x")

    def test_semantica_distingue_identificadores_de_literais_e_rotulos_da_ast(self):
        self.conferir_sucesso(self.executar_parser(
            'int Bloco = 1; int If = Bloco; String s = "ausente"; char c = \'x\'; '
            'boolean b = !false || true; System.out.println(Math.pow(If, Math.sqrt(Bloco)));'))
        self.conferir_erro_semantico("System.out.println(If);", "Variável não declarada", "If")

    def test_reporta_multiplos_erros_semanticos_sem_imprimir_sucesso(self):
        resultado = self.conferir_erro_semantico(
            "int x; int x; y = z;", "Variável já declarada", "x")
        self.assertIn("Variável não declarada 'y'", resultado.stderr)
        self.assertIn("Variável não declarada 'z'", resultado.stderr)

    def test_execucao_padrao_nao_imprime_ast_nem_tabela(self):
        resultado = self.executar_parser("int x = 1; x += 2;", debug=False)
        self.assertEqual(resultado.returncode, 0)
        self.assertEqual(resultado.stdout, SUCESSO)
        self.assertEqual(resultado.stderr, "")

    def test_debug_funciona_com_arquivo_e_em_qualquer_ordem(self):
        arquivo = TESTES / "validos" / "declaracoes.java"
        for argumentos in (("--debug", arquivo), (arquivo, "--debug")):
            with self.subTest(argumentos=argumentos):
                self.conferir_sucesso(self.executar_parser(argumentos=argumentos, debug=False))
        resultado = self.executar_parser(argumentos=[arquivo], debug=False)
        self.assertEqual(resultado.returncode, 0)
        self.assertEqual(resultado.stdout, SUCESSO)
        self.assertEqual(resultado.stderr, "")

    def test_desativar_debug_preserva_analise_e_diagnosticos(self):
        entradas = ["int x; int x;", "x = 1;", "int x = y;", "int x =;", "int x; @"]
        for entrada in entradas:
            with self.subTest(entrada=entrada):
                com_debug = self.executar_parser(entrada)
                sem_debug = self.executar_parser(entrada, debug=False)
                self.assertEqual(com_debug.returncode, 1)
                self.assertEqual((com_debug.returncode, com_debug.stdout, com_debug.stderr),
                                 (sem_debug.returncode, sem_debug.stdout, sem_debug.stderr))
                self.assertNotIn("AST:", sem_debug.stdout)
                self.assertNotIn("Tabela de simbolos", sem_debug.stdout)

    def test_rejeita_opcoes_desconhecidas_e_multiplos_arquivos(self):
        for argumentos in (("--desconhecido",), ("um.java", "dois.java")):
            with self.subTest(argumentos=argumentos):
                resultado = self.executar_parser(argumentos=argumentos, debug=False)
                self.assertEqual(resultado.returncode, 1)
                self.assertEqual(resultado.stdout, "")
                self.assertIn("[--debug] [arquivo.java]", resultado.stderr)


if __name__ == "__main__":
    unittest.main(verbosity=2)
