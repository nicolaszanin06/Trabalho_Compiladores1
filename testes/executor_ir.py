"""Executor pequeno para testar a IR; nao e um backend do compilador."""
import ast
import math
import struct
import sys


def inteiro32(valor):
    return (valor + 2**31) % 2**32 - 2**31


def float32(valor):
    return struct.unpack("f", struct.pack("f", valor))[0]


def literal(texto):
    if texto in ("true", "false"):
        return "boolean", texto == "true"
    if texto.startswith('"'):
        return "String", sys.intern(ast.literal_eval(texto))
    if texto.startswith("'"):
        return "char", ast.literal_eval(texto)
    if "." in texto:
        return "float", float32(float(texto))
    valor = int(texto, 10)
    if not -(2**31) <= valor < 2**31:
        raise ValueError("Constante int fora da faixa")
    return "int", valor


def dividir_inteiros(a, b):
    quociente = abs(a) // abs(b)
    return -quociente if (a < 0) != (b < 0) else quociente


def executar_ir(codigo, limite_passos=10000):
    """Retorna a impressao do programa e exige pilha vazia ao terminar."""
    instrucoes = [linha.split(" ", 1) for linha in codigo.splitlines() if linha]
    rotulos = {}
    for indice, instrucao in enumerate(instrucoes):
        if instrucao[0] == "LABEL":
            nome = instrucao[1]
            if nome in rotulos:
                raise ValueError("Rotulo duplicado")
            rotulos[nome] = indice

    pilha = []
    memoria = {}
    saida = []
    pc = 0
    passos = 0

    def retirar():
        if not pilha:
            raise ValueError("Pilha vazia")
        return pilha.pop()

    while pc < len(instrucoes):
        if passos >= limite_passos:
            raise RuntimeError("Limite de passos da IR excedido")
        passos += 1
        instrucao = instrucoes[pc]
        opcode = instrucao[0]
        argumento = instrucao[1] if len(instrucao) == 2 else None
        pc += 1

        if opcode == "LOAD_CONST":
            pilha.append(literal(argumento))
        elif opcode == "LOAD":
            pilha.append(memoria[argumento])
        elif opcode == "STORE":
            memoria[argumento] = retirar()
        elif opcode == "LABEL":
            pass
        elif opcode == "JUMP":
            pc = rotulos[argumento]
        elif opcode == "JUMP_IF_FALSE":
            tipo, valor = retirar()
            if tipo != "boolean":
                raise ValueError("Desvio exige boolean")
            if not valor:
                pc = rotulos[argumento]
        elif opcode in ("PRINT", "PRINTLN"):
            tipo, valor = retirar()
            texto = ("true" if valor else "false") if tipo == "boolean" else str(valor)
            saida.append(texto + ("\n" if opcode == "PRINTLN" else ""))
        elif opcode in ("NEG", "NOT", "I2F", "SQRT"):
            tipo, valor = retirar()
            if opcode == "NOT":
                pilha.append(("boolean", not valor))
            elif opcode == "I2F":
                pilha.append(("float", float32(valor)))
            elif opcode == "SQRT":
                pilha.append(("float", float32(math.sqrt(valor))))
            else:
                pilha.append((tipo, inteiro32(-valor) if tipo == "int" else float32(-valor)))
        else:
            tipo_b, b = retirar()
            tipo_a, a = retirar()
            if tipo_a != tipo_b:
                raise ValueError("Operandos sem conversao explicita")
            if opcode in ("EQ", "NE", "LT", "LE", "GT", "GE"):
                igualdade = a is b if tipo_a == "String" else a == b
                comparacoes = {"EQ": lambda: igualdade, "NE": lambda: not igualdade,
                               "LT": lambda: a < b, "LE": lambda: a <= b,
                               "GT": lambda: a > b, "GE": lambda: a >= b}
                pilha.append(("boolean", comparacoes[opcode]()))
                continue
            if opcode == "ADD":
                valor = a + b
            elif opcode == "SUB":
                valor = a - b
            elif opcode == "MUL":
                valor = a * b
            elif opcode == "DIV":
                valor = dividir_inteiros(a, b) if tipo_a == "int" else a / b
            elif opcode == "MOD":
                valor = a - dividir_inteiros(a, b) * b if tipo_a == "int" else math.fmod(a, b)
            elif opcode == "POW":
                valor = math.pow(a, b)
            else:
                raise ValueError(f"Instrucao desconhecida: {opcode}")
            pilha.append((tipo_a, inteiro32(valor) if tipo_a == "int" else float32(valor)))

    if pilha:
        raise ValueError("Pilha nao vazia ao terminar")
    return "".join(saida)
