#include "buscas.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static size_t limite_binario(size_t n) {
    size_t comparacoes = 0;
    while (n > 0) {
        ++comparacoes;
        n /= 2;
    }
    return comparacoes;
}

static void testar_vazio(void) {
    ResultadoBusca a = busca_sequencial(NULL, 0, 3);
    ResultadoBusca b = busca_binaria(NULL, 0, 3);
    assert(a.indice == -1 && a.comparacoes == 0);
    assert(b.indice == -1 && b.comparacoes == 0);
}

static void testar_um_elemento(void) {
    const int vetor[] = {7};
    ResultadoBusca a = busca_sequencial(vetor, 1, 7);
    ResultadoBusca b = busca_binaria(vetor, 1, 7);
    assert(a.indice == 0 && a.comparacoes == 1);
    assert(b.indice == 0 && b.comparacoes == 1);

    a = busca_sequencial(vetor, 1, 9);
    b = busca_binaria(vetor, 1, 9);
    assert(a.indice == -1 && a.comparacoes == 1);
    assert(b.indice == -1 && b.comparacoes == 1);
}

static void testar_duplicatas_e_negativos(void) {
    const int vetor[] = {-8, -4, -4, -4, 0, 9, 22};
    ResultadoBusca a = busca_sequencial(vetor, 7, -4);
    ResultadoBusca b = busca_binaria(vetor, 7, -4);
    assert(a.indice == 1 && a.comparacoes == 2);
    assert(b.indice >= 1 && b.indice <= 3 && vetor[b.indice] == -4);

    a = busca_sequencial(vetor, 7, -20);
    b = busca_binaria(vetor, 7, -20);
    assert(a.indice == -1 && a.comparacoes == 7);
    assert(b.indice == -1 && b.comparacoes <= limite_binario(7));
}

static void testar_tamanhos_do_experimento(void) {
    const size_t tamanhos[] = {10, 50, 100, 500, 1000, 5000};

    for (size_t t = 0; t < sizeof(tamanhos) / sizeof(tamanhos[0]); ++t) {
        size_t n = tamanhos[t];
        int *vetor = malloc(n * sizeof(*vetor));
        assert(vetor != NULL);

        for (size_t i = 0; i < n; ++i) {
            vetor[i] = (int)(i * 2);
        }

        const int presentes[] = {vetor[0], vetor[n / 2], vetor[n - 1]};
        for (size_t i = 0; i < 3; ++i) {
            ResultadoBusca a = busca_sequencial(vetor, n, presentes[i]);
            ResultadoBusca b = busca_binaria(vetor, n, presentes[i]);
            assert(a.indice >= 0 && vetor[a.indice] == presentes[i]);
            assert(b.indice >= 0 && vetor[b.indice] == presentes[i]);
            assert(a.comparacoes <= n);
            assert(b.comparacoes <= limite_binario(n));
        }

        const int ausentes[] = {-1, 1, vetor[n - 1] + 3};
        for (size_t i = 0; i < 3; ++i) {
            ResultadoBusca a = busca_sequencial(vetor, n, ausentes[i]);
            ResultadoBusca b = busca_binaria(vetor, n, ausentes[i]);
            assert(a.indice == -1 && a.comparacoes == n);
            assert(b.indice == -1 && b.comparacoes <= limite_binario(n));
        }

        /* Alvo acima do maior valor atinge a profundidade maxima
           nesta implementacao de busca binaria com intervalo fechado. */
        ResultadoBusca pior = busca_binaria(vetor, n, vetor[n - 1] + 3);
        assert(pior.comparacoes == limite_binario(n));
        free(vetor);
    }
}

int main(void) {
    testar_vazio();
    testar_um_elemento();
    testar_duplicatas_e_negativos();
    testar_tamanhos_do_experimento();
    puts("OK: testes de busca sequencial e binaria passaram.");
    return 0;
}
