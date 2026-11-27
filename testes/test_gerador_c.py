"""Testes end-to-end do backend C: Java Procedural -> C11 -> executavel."""

import os
from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
COMPILADOR = Path(os.environ.get("JTOC", ROOT / "src" / ("jtoc.exe" if os.name == "nt" else "jtoc")))
CC = os.environ.get("CC", "gcc")


class GeradorCTest(unittest.TestCase):
    def traduz_e_executa(self, fonte, saida_esperada, retorno_esperado=0):
        traducao = subprocess.run(
            [str(COMPILADOR), "--c"],
            input=fonte,
            text=True,
            capture_output=True,
            timeout=10,
        )
        self.assertEqual(traducao.returncode, 0, traducao.stderr)
        self.assertNotEqual(traducao.stdout, "")

        with tempfile.TemporaryDirectory(prefix="jtoc-c-") as pasta:
            pasta = Path(pasta)
            arquivo_c = pasta / "programa.c"
            executavel = pasta / ("programa.exe" if os.name == "nt" else "programa")
            arquivo_c.write_text(traducao.stdout, encoding="utf-8")

            compilacao = subprocess.run(
                [CC, "-std=c11", "-Wall", "-Wextra", "-Wpedantic",
                 str(arquivo_c), "-o", str(executavel), "-lm"],
                text=True,
                capture_output=True,
                timeout=30,
            )
            self.assertEqual(compilacao.returncode, 0,
                             compilacao.stdout + compilacao.stderr)
            self.assertNotIn("-Warray-compare", compilacao.stderr)

            execucao = subprocess.run(
                [str(executavel)],
                text=True,
                capture_output=True,
                timeout=5,
            )
            self.assertEqual(execucao.stdout, saida_esperada)
            self.assertEqual(execucao.returncode, retorno_esperado, execucao.stderr)
            return execucao

    def test_precedencia_e_println(self):
        self.traduz_e_executa(
            "int resultado = 2 + 3 * 4; System.out.println(resultado);",
            "14\n",
        )

    def test_normalizacao_decimal_e_overflow(self):
        self.traduz_e_executa(
            "int valor = 007; valor += 2147483647; valor++; "
            "System.out.println(valor);",
            "-2147483641\n",
        )

    def test_minimo_divisao_e_resto_especiais(self):
        self.traduz_e_executa(
            "int minimo = -2147483648; "
            "System.out.println(minimo / -1); "
            "System.out.println(minimo % -1);",
            "-2147483648\n0\n",
        )

    def test_curto_circuito(self):
        self.traduz_e_executa(
            "boolean resultado = false && (1 / 0 > 0); "
            "System.out.println(resultado);",
            "false\n",
        )

    def test_for_e_atualizacoes(self):
        self.traduz_e_executa(
            "int soma = 0; "
            "for (int i = 0; i < 5; i++) { soma += i; } "
            "System.out.println(soma);",
            "10\n",
        )

    def test_switch_fallthrough(self):
        self.traduz_e_executa(
            "int opcao = 2; int resultado = 0; "
            "switch (opcao) { "
            "case 1: resultado = 1; "
            "case 2: resultado = 2; "
            "default: resultado = 3; } "
            "System.out.println(resultado);",
            "3\n",
        )

    def test_identidade_de_strings(self):
        self.traduz_e_executa(
            'String a = "mesmo"; String b = "mesmo"; '
            'System.out.println(a == b); System.out.println("100% pronto");',
            "true\n100% pronto\n",
        )

    def test_float_e_matematica(self):
        self.traduz_e_executa(
            "float resto = 5.5 % 2.0; "
            "System.out.println(resto); "
            "System.out.println(Math.sqrt(9));",
            "1.500000\n3.000000\n",
        )

    def test_comparacao_de_literais_string_sem_warning_de_arrays(self):
        self.traduz_e_executa(
            'System.out.println("x" == "x"); '
            'System.out.println("x" != "y"); '
            'System.out.println("x" == "y"); '
            'String a = "x"; String b = a; '
            'System.out.println(a == b); '
            r'''System.out.println("a'b" == "a\'b"); '''
            'String c = "y"; System.out.println(a == c); '
            'System.out.println(a == "x"); System.out.println("x" == a);',
            "true\ntrue\nfalse\ntrue\ntrue\nfalse\ntrue\ntrue\n",
        )

    def test_leitura_sem_inicializacao_nao_emite_c(self):
        resultado = subprocess.run(
            [str(COMPILADOR), "--c"],
            input="int x; System.out.println(x);",
            text=True,
            capture_output=True,
            timeout=10,
        )
        self.assertNotEqual(resultado.returncode, 0)
        self.assertEqual(resultado.stdout, "")
        self.assertIn("inicializada", resultado.stderr)


    def test_divisao_inteira_por_zero_termina_com_erro(self):
        traducao = subprocess.run(
            [str(COMPILADOR), "--c"],
            input="int zero = 0; System.out.println(10 / zero);",
            text=True,
            capture_output=True,
            timeout=10,
        )
        self.assertEqual(traducao.returncode, 0, traducao.stderr)
        with tempfile.TemporaryDirectory(prefix="jtoc-divzero-") as pasta:
            arquivo_c = Path(pasta) / "programa.c"
            executavel = Path(pasta) / ("programa.exe" if os.name == "nt" else "programa")
            arquivo_c.write_text(traducao.stdout, encoding="utf-8")
            compilacao = subprocess.run(
                [CC, "-std=c11", str(arquivo_c), "-o", str(executavel)],
                text=True,
                capture_output=True,
                timeout=30,
            )
            self.assertEqual(compilacao.returncode, 0, compilacao.stderr)
            execucao = subprocess.run(
                [str(executavel)], text=True, capture_output=True, timeout=5
            )
            self.assertNotEqual(execucao.returncode, 0)
            self.assertIn("divisao inteira por zero", execucao.stderr)

    def test_destino_existente_nao_e_sobrescrito(self):
        with tempfile.TemporaryDirectory(prefix="jtoc-destino-") as pasta:
            destino = Path(pasta) / "programa.c"
            destino.write_text("nao sobrescrever", encoding="utf-8")
            resultado = subprocess.run(
                [str(COMPILADOR), "--c", "-o", str(destino)],
                input="System.out.println(1);",
                text=True,
                capture_output=True,
                timeout=10,
            )
            self.assertNotEqual(resultado.returncode, 0)
            self.assertEqual(destino.read_text(encoding="utf-8"), "nao sobrescrever")

    def test_opcoes_c_em_qualquer_ordem(self):
        combinacoes = (
            ("--c", "-o", "{destino}"),
            ("-o", "{destino}", "--c"),
            ("-o", "{destino}"),
            ("--debug", "-o", "{destino}", "--c"),
            ("--c", "--c", "-o", "{destino}"),
        )
        with tempfile.TemporaryDirectory(prefix="jtoc-cli-") as pasta:
            codigos = []
            for indice, combinacao in enumerate(combinacoes):
                destino = Path(pasta) / f"programa-{indice}.c"
                argumentos = [arg.format(destino=destino) for arg in combinacao]
                with self.subTest(argumentos=argumentos):
                    resultado = subprocess.run(
                        [str(COMPILADOR), *argumentos],
                        input="System.out.println(7);", text=True,
                        capture_output=True, timeout=10,
                    )
                    self.assertEqual(resultado.returncode, 0, resultado.stderr)
                    self.assertEqual(resultado.stdout, "")
                    codigo = destino.read_text(encoding="utf-8")
                    self.assertIn("int main(void)", codigo)
                    codigos.append(codigo)
                    if "--debug" in combinacao:
                        self.assertIn("AST:", resultado.stderr)
                        self.assertIn("Tabela de simbolos", resultado.stderr)
                        self.assertNotIn("AST:", codigo)
                    else:
                        self.assertEqual(resultado.stderr, "")
            self.assertTrue(all(codigo == codigos[0] for codigo in codigos))

    def test_repeticao_do_mesmo_modo_e_idempotente(self):
        for modo in ("--c", "--ir"):
            with self.subTest(modo=modo):
                resultados = [subprocess.run(
                    [str(COMPILADOR), *([modo] * vezes)],
                    input="int x = 1;", text=True, capture_output=True, timeout=10,
                ) for vezes in (1, 2)]
                for resultado in resultados:
                    self.assertEqual(resultado.returncode, 0, resultado.stderr)
                    self.assertEqual(resultado.stderr, "")
                self.assertEqual(resultados[0].stdout, resultados[1].stdout)

    def test_opcoes_de_geracao_sao_exclusivas(self):
        combinacoes = (
            ("--c", "--ir"), ("--ir", "--c"),
            ("--ir", "-o", "{destino}"),
            ("-o", "{destino}", "--ir"),
            ("--c", "-o", "{destino}", "--ir"),
            ("-o",), ("--c", "-o"),
            ("-o", "{destino}", "-o", "{destino}"),
        )
        with tempfile.TemporaryDirectory(prefix="jtoc-cli-invalida-") as pasta:
            destino = Path(pasta) / "programa.c"
            for combinacao in combinacoes:
                argumentos = [arg.format(destino=destino) for arg in combinacao]
                with self.subTest(argumentos=argumentos):
                    resultado = subprocess.run(
                        [str(COMPILADOR), *argumentos],
                        input="System.out.println(7);", text=True,
                        capture_output=True, timeout=10,
                    )
                    self.assertNotEqual(resultado.returncode, 0)
                    self.assertEqual(resultado.stdout, "")
                    self.assertIn("Uso:", resultado.stderr)
                    self.assertFalse(destino.exists())

    def test_debug_nao_contamina_stdout(self):
        resultado = subprocess.run(
            [str(COMPILADOR), "--c", "--debug"],
            input="System.out.println(7);",
            text=True,
            capture_output=True,
            timeout=10,
        )
        self.assertEqual(resultado.returncode, 0, resultado.stderr)
        self.assertIn("int main(void)", resultado.stdout)
        self.assertIn("AST:", resultado.stderr)
        self.assertNotIn("AST:", resultado.stdout)


if __name__ == "__main__":
    unittest.main()
