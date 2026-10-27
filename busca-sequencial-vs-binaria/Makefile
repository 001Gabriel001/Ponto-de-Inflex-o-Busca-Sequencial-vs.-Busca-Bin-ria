CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror
CPPFLAGS = -Iinclude
LDLIBS = -lm

.DEFAULT_GOAL := all
.PHONY: all compilar run test dados grafico clean

# Digitar 'make' compila e abre as perguntas ao usuario.
all: run

bin:
	mkdir -p bin

bin/comparador: src/main.c src/buscas.c include/buscas.h | bin
	$(CC) $(CFLAGS) $(CPPFLAGS) src/main.c src/buscas.c -o $@ $(LDLIBS)

bin/experimento: src/experimento.c src/buscas.c include/buscas.h | bin
	$(CC) $(CFLAGS) $(CPPFLAGS) src/experimento.c src/buscas.c -o $@ $(LDLIBS)

bin/test_buscas: tests/test_buscas.c src/buscas.c include/buscas.h | bin
	$(CC) $(CFLAGS) $(CPPFLAGS) tests/test_buscas.c src/buscas.c -o $@

compilar: bin/comparador bin/experimento

run: bin/comparador
	./bin/comparador

test: bin/test_buscas bin/comparador
	./bin/test_buscas
	sh tests/test_interativo.sh

dados: bin/experimento
	./bin/experimento dados/resultados.csv

grafico: dados
	awk -f scripts/gerar_grafico.awk dados/resultados.csv > docs/grafico.svg

clean:
	rm -rf bin resultados.csv resultado_busca.csv
