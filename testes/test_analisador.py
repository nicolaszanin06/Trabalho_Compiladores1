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
        for nome in ("scanner.l", "scanner.y", "scanner.tab.c", "scanner.tab.h", "lex.yy.c"):
            shutil.copy2(SRC / nome, pasta / nome)

        def compilar(*comando):
            resultado = subprocess.run(comando, cwd=pasta, capture_output=True,
                                       text=True, timeout=60)
            if resultado.returncode:
                raise RuntimeError(f"Falha em {comando}:\n{resultado.stdout}{resultado.stderr}")

        if REGENERAR:
            compilar("bison", "-d", "scanner.y")
            compilar("flex", "scanner.l")
        sufixo = ".exe" if os.name == "nt" else ""
        cls.parser = pasta / ("parser" + sufixo)
        cls.lexer = pasta / ("lexer" + sufixo)
        cc = os.environ.get("CC", "gcc")
        compilar(cc, "-o", str(cls.parser), "scanner.tab.c", "lex.yy.c")
        compilar(cc, "-I", str(pasta), "-o", str(cls.lexer),
                 str(TESTES / "lexer_driver.c"), "lex.yy.c")

    def executar(self, executavel, entrada="", argumentos=()):
        return subprocess.run([str(executavel), *map(str, argumentos)], input=entrada,
                              capture_output=True, text=True, timeout=5)

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

    def test_literais_e_valor_semantico_inteiro(self):
        self.conferir_lexer('0 007 123 12.50 \'a\' \'\\n\' "ola" "a\\\"b"',
                            'INT_LITERAL|0|1|0\nINT_LITERAL|007|1|7\nINT_LITERAL|123|1|123\nFLOAT_LITERAL|12.50|1\nCHAR_LITERAL|\'a\'|1\nCHAR_LITERAL|\'\\n\'|1\nSTRING_LITERAL|"ola"|1\nSTRING_LITERAL|"a\\\"b"|1\n')

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
                resultado = self.executar(self.parser, argumentos=[arquivo])
                self.assertEqual((resultado.returncode, resultado.stdout, resultado.stderr),
                                 (0, SUCESSO, ""))

    def test_arquivos_com_erros_sintaticos(self):
        for nome in ("sem_ponto_e_virgula", "atribuicao_na_declaracao", "condicao_nao_suportada"):
            with self.subTest(arquivo=nome):
                resultado = self.executar(self.parser, argumentos=[TESTES / "invalidos" / (nome + ".java")])
                self.assertEqual(resultado.returncode, 0)  # Comportamento atual.
                self.assertEqual(resultado.stdout, "")
                self.assertRegex(resultado.stderr, r"\AErro sintatico na linha \d+: syntax error perto de '.*'\n\Z")

    def test_erros_lexicos_ainda_imprimem_sucesso(self):
        for nome, mensagem in (("caractere_invalido", "Caractere invalido '@'"),
                               ("string_nao_encerrada", "String nao encerrada")):
            with self.subTest(arquivo=nome):
                arquivo = TESTES / "invalidos" / (nome + ".java")
                resultado = self.executar(self.parser, argumentos=[arquivo])
                self.assertEqual(resultado.returncode, 0)
                self.assertEqual(resultado.stderr, "")
                # A regra atual da string consome tambem a quebra de linha.
                linha = 1 if nome == "caractere_invalido" else 1 + arquivo.read_text().count("\n")
                self.assertEqual(resultado.stdout, f"Erro lexico na linha {linha}: {mensagem}\n" + SUCESSO)

    def test_blocos_vazios_e_declaracoes_sem_espacos(self):
        resultado = self.executar(self.parser, "if(true){}if(false){int x;float y;char z;}")
        self.assertEqual((resultado.returncode, resultado.stdout, resultado.stderr), (0, SUCESSO, ""))

    def test_erros_sintaticos_com_linha_e_token_exatos(self):
        casos = [("int x\nfloat y;", 2, "float"),
                 ("if (true) {\nint x;", 2, ""),
                 ("int x;\n}", 2, "}"),
                 ("if (true) {} else {}", 1, "else"),
                 ("boolean x;", 1, "boolean")]
        for entrada, linha, token in casos:
            with self.subTest(entrada=entrada):
                resultado = self.executar(self.parser, entrada)
                self.assertEqual(resultado.returncode, 0)
                self.assertEqual(resultado.stdout, "")
                self.assertEqual(resultado.stderr, f"Erro sintatico na linha {linha}: syntax error perto de '{token}'\n")

    def test_arquivo_inexistente(self):
        resultado = self.executar(self.parser, argumentos=[Path(self.temporario.name) / "inexistente.java"])
        self.assertEqual(resultado.returncode, 1)
        self.assertEqual(resultado.stdout, "")
        self.assertIn("Erro ao abrir o arquivo", resultado.stderr)


if __name__ == "__main__":
    unittest.main(verbosity=2)
