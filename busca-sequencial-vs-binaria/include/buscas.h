#ifndef BUSCAS_H
#define BUSCAS_H

#include <stddef.h>

/*
 * Um resultado armazena o indice encontrado (ou -1 se ausente)
 * e o numero de comparacoes logicas realizadas.
 *
 * Uma comparacao e uma chamada ao comparador de dois inteiros.
 * O numero nao equivale necessariamente a instrucoes de CPU ou a
 * operadores relacionais avaliados internamente pelo comparador.
 */
typedef struct {
    ptrdiff_t indice;
    size_t comparacoes;
} ResultadoBusca;

/* Nao exige ordenacao. */
ResultadoBusca busca_sequencial(const int *vetor, size_t tamanho, int alvo);

/* Exige vetor ordenado em ordem crescente (valores repetidos permitidos). */
ResultadoBusca busca_binaria(const int *vetor, size_t tamanho, int alvo);

#endif
