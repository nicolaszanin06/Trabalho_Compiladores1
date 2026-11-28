"""Testes end-to-end do backend C: Java Procedural -> C11 -> executavel."""

import os
from pathlib import Path
import subprocess
import tempfile
import time
import unittest


ROOT = Path(__file__).resolve().parents[1]
COMPILADOR = Path(os.environ.get("JTOC", ROOT / "src" / ("jtoc.exe" if os.name == "nt" else "jtoc")))
CC = os.environ.get("CC", "gcc")


class GeradorCTest(unittest.TestCase):
    def traduz_e_executa(self, fonte, saida_esperada, *,
                         exigir_sem_warnings=False, erro_esperado=""):
        traducao = subprocess.run(
            [str(COMPILADOR), "--c"],
            input=fonte,
            text=True,
            encoding="utf-8",
            capture_output=True,
            timeout=10,
        )
        self.assertEqual(traducao.returncode, 0, traducao.stderr)
        self.assertNotEqual(traducao.stdout, "")
        self.assertEqual(traducao.stderr, "")

        with tempfile.TemporaryDirectory(prefix="jtoc-c-") as pasta:
            arquivo_c = Path(pasta) / "programa.c"
            arquivo_c.write_text(traducao.stdout, encoding="utf-8")
            self.compila_e_executa(arquivo_c, saida_esperada,
                                  exigir_sem_warnings=exigir_sem_warnings,
                                  erro_esperado=erro_esperado)

    def compila_e_executa(self, arquivo_c, saida_esperada, *,
                          exigir_sem_warnings=False, erro_esperado=""):
        executavel = arquivo_c.with_name("programa.exe" if os.name == "nt" else "programa")
        compilacao = subprocess.run(
            [CC, "-std=c11", "-Wall", "-Wextra", "-Wpedantic",
             str(arquivo_c), "-o", str(executavel), "-lm"],
            text=True,
            encoding="utf-8",
            capture_output=True,
            timeout=30,
        )
        self.assertEqual(compilacao.returncode, 0, compilacao.stdout + compilacao.stderr)
        self.assertNotIn("-Warray-compare", compilacao.stderr)
        if exigir_sem_warnings:
            self.assertEqual(compilacao.stderr, "")

        try:
            execucao = subprocess.run(
                [str(executavel)],
                text=True,
                encoding="utf-8",
                capture_output=True,
                timeout=5,
            )
        finally:
            # No Windows, o executavel pode ficar bloqueado logo apos terminar.
            for tentativa in range(6):
                try:
                    executavel.unlink(missing_ok=True)
                    break
                except PermissionError as erro:
                    if getattr(erro, "winerror", None) != 32 or tentativa == 5:
                        raise
                    time.sleep(0.1)
        self.assertEqual(execucao.stdout, saida_esperada)
        self.assertEqual(execucao.stderr, erro_esperado)
        if erro_esperado:
            self.assertNotEqual(execucao.returncode, 0)
        else:
            self.assertEqual(execucao.returncode, 0)

    def test_precedencia_e_println(self):
        self.traduz_e_executa(
            "int resultado = 2 + 3 * 4; System.out.println(resultado);",
            "14\n",
            exigir_sem_warnings=True,
        )

    def test_normalizacao_decimal_e_overflow(self):
        self.traduz_e_executa(
            "int valor = 007; valor += 2147483647; valor++; "
            "System.out.println(valor);",
            "-2147483641\n",
            exigir_sem_warnings=True,
        )

    def test_minimo_divisao_e_resto_especiais(self):
        self.traduz_e_executa(
            "int minimo = -2147483648; "
            "System.out.println(minimo / -1); "
            "System.out.println(minimo % -1);",
            "-2147483648\n0\n",
        )

    def test_divisao_trunca_e_resto_preserva_sinal_do_dividendo(self):
        casos = ((-7, 3, "-2\n-1\n"), (7, -3, "-2\n1\n"), (-7, -3, "2\n-1\n"))
        for dividendo, divisor, esperado in casos:
            with self.subTest(dividendo=dividendo, divisor=divisor):
                self.traduz_e_executa(
                    f"int a = {dividendo}; int b = {divisor}; "
                    "System.out.println(a / b); System.out.println(a % b);",
                    esperado,
                )

    def test_overflow_em_subtracao_multiplicacao_e_negacao(self):
        self.traduz_e_executa(
            "int minimo = -2147483648; int maximo = 2147483647; "
            "System.out.println(minimo - 1); System.out.println(maximo * 2); "
            "System.out.println(-minimo); minimo--; System.out.println(minimo);",
            "2147483647\n-2\n-2147483648\n2147483647\n",
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
            exigir_sem_warnings=True,
        )

    def test_float_e_matematica(self):
        self.traduz_e_executa(
            "float resto = 5.5 % 2.0; "
            "System.out.println(resto); "
            "System.out.println(Math.sqrt(9));",
            "1.500000\n3.000000\n",
            exigir_sem_warnings=True,
        )

    def test_divisao_mista_e_promocao_para_float(self):
        self.traduz_e_executa(
            "int total = 7; int divisor = 2; float fracao = 2.0; "
            "float inteira = total / divisor; "
            "System.out.println(inteira); System.out.println(total / fracao); "
            "fracao += total; System.out.println(fracao);",
            "3.000000\n3.500000\n9.000000\n",
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

    def test_matematica_aninhada(self):
        self.traduz_e_executa(
            "System.out.println(Math.pow(2, 3)); "
            "System.out.println(Math.pow(2, Math.sqrt(9))); "
            "System.out.println(Math.sqrt(Math.pow(3, 2)));",
            "8.000000\n8.000000\n3.000000\n",
            exigir_sem_warnings=True,
        )

    def test_float_limite_superior_e_subnormais_sem_warnings(self):
        literais = ("340282346638528859811704183484516925440.0",
                    "0." + "0" * 39 + "1", "0." + "0" * 44 + "1",
                    "0." + "0" * 45 + "8")
        for literal in literais:
            with self.subTest(literal=literal):
                self.traduz_e_executa(
                    f"float x = {literal}; System.out.println(x > 0.0); "
                    f"float y = -{literal}; System.out.println(y < 0.0);",
                    "true\ntrue\n", exigir_sem_warnings=True)
        self.traduz_e_executa(
            "float x = 0." + "0" * 60 + "; System.out.println(x == 0.0);",
            "true\n", exigir_sem_warnings=True)

    def test_float_fora_da_faixa_nao_emite_c_nem_cria_destino(self):
        with tempfile.TemporaryDirectory(prefix="jtoc-float-") as pasta:
            destino = Path(pasta) / "programa.c"
            for literal in ("9" * 45 + ".0", "0." + "0" * 60 + "1"):
                for argumentos in (("--c",), ("-o", str(destino))):
                    with self.subTest(literal=literal, argumentos=argumentos):
                        resultado = subprocess.run(
                            [str(COMPILADOR), *argumentos],
                            input=f"float x = {literal};", text=True,
                            capture_output=True, timeout=10)
                        self.assertEqual(resultado.returncode, 1)
                        self.assertEqual(resultado.stdout, "")
                        self.assertIn("literal float fora da faixa", resultado.stderr)
                        self.assertFalse(destino.exists())

    def test_variavel_nao_utilizada_continua_compilavel(self):
        # GCC pode avisar sobre x; esse programa nao exige stderr vazio no build.
        self.traduz_e_executa("int x = 1;", "")

    def test_if_else_executa_ambos_os_ramos(self):
        for condicao, esperado in (("true", "1\n"), ("false", "2\n")):
            with self.subTest(condicao=condicao):
                self.traduz_e_executa(
                    f"boolean flag = {condicao}; "
                    "if (flag) { System.out.println(1); } "
                    "else { System.out.println(2); }",
                    esperado,
                )

    def test_inicializacao_em_condicoes_constantes(self):
        self.traduz_e_executa(
            "int x; if (1 == 1) { x = 2; } System.out.println(x); "
            "int y; while (2 > 1) { y = 3; break; } System.out.println(y);",
            "2\n3\n",
        )

    def test_lacos_aninhados_com_break(self):
        self.traduz_e_executa(
            "int soma = 0; for (int i = 0; i < 3; i++) { int j = 0; "
            "while (j < 4) { j++; if (j == 3) { break; } soma += i + j; } } "
            "System.out.println(soma);",
            "15\n",
        )

    def test_for_com_clausulas_opcionais(self):
        self.traduz_e_executa(
            "int i; for (i = 0; i < 2; i += 1) { System.out.print(i); } "
            "for (; i > 0; i -= 1) {} for (;;) { break; } System.out.println(i);",
            "010\n",
        )

    def test_escopos_e_nomes_reservados_no_alvo_c(self):
        self.traduz_e_executa(
            "int auto = 1; { int auto = 2; System.out.println(auto); } "
            "{ int auto = 3; System.out.println(auto); } "
            "int printf = 3; int Bloco = 4; System.out.println(auto + printf + Bloco);",
            "2\n3\n8\n",
        )

    def test_switch_default_intermediario_e_cases_agrupados(self):
        self.traduz_e_executa(
            "switch (2) { default: System.out.print(0); break; "
            "case 1: case 2: System.out.print(2); case 3: System.out.println(3); } "
            "switch (9) { case 1: System.out.print(1); break; "
            "default: System.out.print(9); case 2: System.out.println(2); }",
            "23\n92\n",
        )

    def test_switch_preserva_escopo_entre_cases(self):
        self.traduz_e_executa(
            "switch (2) { case 1: int x = 1; "
            "case 2: x = 2; System.out.println(x); break; }",
            "2\n",
        )

    def test_break_em_switch_dentro_de_laco(self):
        self.traduz_e_executa(
            "for (int i = 0; i < 3; i++) { switch (i) { case 0: break; "
            "default: System.out.print(i); } if (i == 1) { break; } } "
            "switch (1) { case 1: switch (2) { case 2: System.out.print(2); break; } "
            "System.out.println(1); break; }",
            "121\n",
        )

    def test_texto_utf8_formatos_e_escapes(self):
        self.traduz_e_executa(
            r'''String texto = "a\"b\\c\n100% ç"; System.out.println(texto);
                System.out.print('\t'); System.out.println('\'');''',
            'a"b\\c\n100% ç\n\t\'\n',
        )

    def test_curto_circuito_com_disjuncao(self):
        self.traduz_e_executa(
            "int zero = 0; System.out.println(zero == 0 || 10 / zero > 1); "
            "System.out.println(zero != 0 && 10 / zero > 1);",
            "true\nfalse\n",
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

    def test_divisao_e_resto_por_zero_interrompem_execucao(self):
        for comando in ("System.out.println(x / zero);", "System.out.println(x % zero);",
                        "x /= zero;"):
            with self.subTest(comando=comando):
                self.traduz_e_executa(
                    "int x = 10; int zero = 0; System.out.println(x); "
                    f"{comando} System.out.println(99);",
                    "10\n",
                    erro_esperado="Erro de execucao: divisao inteira por zero.\n",
                )

    def test_falha_com_debug_nao_publica_codigo_ou_destino(self):
        casos = (
            (r'System.out.print("\q");', "Escape invalido"),
            ("int x = ;", "Erro sintatico"),
            ("int x; System.out.println(x);", "inicializada"),
        )
        with tempfile.TemporaryDirectory(prefix="jtoc-c-invalido-") as pasta:
            destino = Path(pasta) / "programa.c"
            for fonte, diagnostico in casos:
                for argumentos in (("--c",), ("-o", str(destino))):
                    with self.subTest(fonte=fonte, argumentos=argumentos):
                        resultado = subprocess.run(
                            [str(COMPILADOR), "--debug", *argumentos],
                            input="System.out.println(7); " + fonte,
                            text=True, encoding="utf-8", capture_output=True, timeout=10,
                        )
                        self.assertEqual(resultado.returncode, 1)
                        self.assertEqual(resultado.stdout, "")
                        self.assertIn(diagnostico, resultado.stderr)
                        self.assertNotIn("AST:", resultado.stderr)
                        self.assertFalse(destino.exists())

    def test_arquivo_de_entrada_e_destino_com_espacos(self):
        with tempfile.TemporaryDirectory(prefix="jtoc-cli-arquivos-") as pasta:
            entrada = Path(pasta) / "codigo fonte.java"
            destino = Path(pasta) / "codigo gerado.c"
            entrada.write_text('System.out.println("100% pronto");', encoding="utf-8")
            resultado = subprocess.run(
                [str(COMPILADOR), str(entrada), "--debug", "-o", str(destino)],
                stdin=subprocess.DEVNULL, text=True, encoding="utf-8",
                capture_output=True, timeout=10,
            )
            self.assertEqual(resultado.returncode, 0, resultado.stderr)
            self.assertEqual(resultado.stdout, "")
            self.assertIn("AST:", resultado.stderr)
            self.assertIn("Tabela de simbolos", resultado.stderr)
            self.compila_e_executa(destino, "100% pronto\n", exigir_sem_warnings=True)

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
            self.assertEqual(resultado.returncode, 1)
            self.assertEqual(resultado.stdout, "")
            self.assertIn("sem sobrescrever", resultado.stderr)
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

    def test_o_rejeita_opcao_no_lugar_do_destino(self):
        with tempfile.TemporaryDirectory(prefix="jtoc-cli-destino-ausente-") as pasta:
            for opcao in ("--ir", "--c", "--debug", "-o", "--desconhecida"):
                with self.subTest(opcao=opcao):
                    resultado = subprocess.run(
                        [str(COMPILADOR.resolve()), "-o", opcao],
                        cwd=pasta, input="System.out.println(7);",
                        text=True, encoding="utf-8", capture_output=True, timeout=10,
                    )
                    self.assertEqual(resultado.returncode, 1, resultado.stderr)
                    self.assertEqual(resultado.stdout, "")
                    self.assertIn("Uso:", resultado.stderr)
                    self.assertEqual(list(Path(pasta).iterdir()), [])

    def test_o_aceita_caminho_explicito_para_nome_com_hifen(self):
        with tempfile.TemporaryDirectory(prefix="jtoc-cli-destino-hifen-") as pasta:
            resultado = subprocess.run(
                [str(COMPILADOR.resolve()), "-o", "./--ir"],
                cwd=pasta, input="System.out.println(7);",
                text=True, encoding="utf-8", capture_output=True, timeout=10,
            )
            self.assertEqual(resultado.returncode, 0, resultado.stderr)
            self.assertEqual(resultado.stdout, "")
            self.assertEqual(resultado.stderr, "")
            codigo = (Path(pasta) / "--ir").read_text(encoding="utf-8")
            self.assertIn("int main(void)", codigo)

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
