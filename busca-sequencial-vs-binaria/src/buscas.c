#include "buscas.h"

/* Uma invocacao equivale a uma comparacao logica (de tres vias). */
static int comparar_inteiros(int elemento, int alvo) {
    if (elemento < alvo) {
        return -1;
    }
    if (elemento > alvo) {
        return 1;
    }
    return 0;
}

ResultadoBusca busca_sequencial(const int *vetor, size_t tamanho, int alvo) {
    ResultadoBusca resultado = {-1, 0};

    if (vetor == NULL || tamanho == 0) {
        return resultado;
    }

    for (size_t i = 0; i < tamanho; ++i) {
        ++resultado.comparacoes;
        if (comparar_inteiros(vetor[i], alvo) == 0) {
            resultado.indice = (ptrdiff_t)i;
            return resultado;
        }
    }

    return resultado;
}

ResultadoBusca busca_binaria(const int *vetor, size_t tamanho, int alvo) {
    ResultadoBusca resultado = {-1, 0};

    if (vetor == NULL || tamanho == 0) {
        return resultado;
    }

    /* Intervalo fechado [esquerda, direita]. */
    size_t esquerda = 0;
    size_t direita = tamanho - 1;

    while (esquerda <= direita) {
        size_t meio = esquerda + (direita - esquerda) / 2;
        ++resultado.comparacoes;
        int ordem = comparar_inteiros(vetor[meio], alvo);

        if (ordem == 0) {
            resultado.indice = (ptrdiff_t)meio;
            return resultado;
        }
        if (ordem < 0) {
            esquerda = meio + 1;
        } else {
            /* Evita subfluxo quando meio == 0 (size_t nao tem sinal). */
            if (meio == 0) {
                break;
            }
            direita = meio - 1;
        }
    }

    return resultado;
}
