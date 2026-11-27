# Nome do executavel final
EXEC = jtoc

# Arquivos-fonte do Bison e do Flex
BISON_FILE = jtoc.y
FLEX_FILE  = jtoc.l

# Arquivos que o Bison vai gerar
BISON_C = jtoc.tab.c
BISON_H = jtoc.tab.h

# Arquivo gerado pelo Flex
FLEX_C = lex.yy.c

# Modulo da arvore sintatica abstrata
AST_C = ast.c
AST_H = ast.h

# Modulo da tabela de simbolos e analise semantica
TABELA_C = tabela_simbolos.c
TABELA_H = tabela_simbolos.h

# Geracao de IR e geracao final de C
GERADOR_IR_C = gerador_intermediario.c
GERADOR_IR_H = gerador_intermediario.h
GERADOR_C_C = gerador_c.c
GERADOR_C_H = gerador_c.h

BISON_FLAGS = -d
FLEX_FLAGS =
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
LDFLAGS = -lfl

all: $(EXEC)

$(EXEC): $(BISON_C) $(FLEX_C) $(AST_C) $(AST_H) $(TABELA_C) $(TABELA_H) \
         $(GERADOR_IR_C) $(GERADOR_IR_H) $(GERADOR_C_C) $(GERADOR_C_H)
	$(CC) $(CFLAGS) -o $@ $(BISON_C) $(FLEX_C) $(AST_C) $(TABELA_C) \
		$(GERADOR_IR_C) $(GERADOR_C_C) $(LDFLAGS)

# Uma receita gera simultaneamente os dois arquivos do Bison (GNU Make >= 4.3).
$(BISON_C) $(BISON_H) &: $(BISON_FILE)
	bison $(BISON_FLAGS) $(BISON_FILE)

$(FLEX_C): $(FLEX_FILE) $(BISON_H)
	flex $(FLEX_FLAGS) $(FLEX_FILE)

clean:
	rm -f $(EXEC) $(BISON_C) $(BISON_H) $(FLEX_C)

.PHONY: all clean
