#include "buscas.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static const size_t TAMANHOS[] = {10, 50, 100, 500, 1000, 5000};

int main(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Uso: %s [caminho_do_csv]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *caminho_csv = (argc == 2) ? argv[1] : "resultados.csv";
    FILE *csv = fopen(caminho_csv, "w");
    if (csv == NULL) {
        perror("Nao foi possivel criar o arquivo CSV");
        return EXIT_FAILURE;
    }

    fprintf(csv,
            "n;comparacoes_sequencial;comparacoes_binaria;"
            "razao_experimental;log2_n;razao_teorica\n");

    puts("Busca Sequencial vs. Busca Binaria | pior caso: alvo ausente e acima do maior valor");
    puts("------------------------------------------------------------------------------------");
    printf("%8s | %12s | %10s | %12s | %12s\n",
           "n", "Sequencial", "Binaria", "Razao exp.", "Razao teor.");
    puts("------------------------------------------------------------------------------------");

    for (size_t i = 0; i < sizeof(TAMANHOS) / sizeof(TAMANHOS[0]); ++i) {
        size_t n = TAMANHOS[i];
        int *vetor = malloc(n * sizeof(*vetor));
        if (vetor == NULL) {
            fprintf(stderr, "Falha ao alocar vetor de tamanho %zu.\n", n);
            fclose(csv);
            return EXIT_FAILURE;
        }

        /* Vetor ja ordenado: 0, 2, 4, ..., 2*(n-1). */
        for (size_t j = 0; j < n; ++j) {
            vetor[j] = (int)(2 * j);
        }

        /* Alvo maior que todos os valores do vetor: busca malsucedida. */
        int alvo = vetor[n - 1] + 3;
        ResultadoBusca sequencial = busca_sequencial(vetor, n, alvo);
        ResultadoBusca binaria = busca_binaria(vetor, n, alvo);

        if (sequencial.indice != -1 || binaria.indice != -1 ||
            sequencial.comparacoes != n || binaria.comparacoes == 0) {
            fprintf(stderr, "Resultado inconsistente para n=%zu.\n", n);
            free(vetor);
            fclose(csv);
            return EXIT_FAILURE;
        }

        double logaritmo = log2((double)n);
        double razao_experimental =
            (double)sequencial.comparacoes / (double)binaria.comparacoes;
        double razao_teorica = (double)n / logaritmo;

        printf("%8zu | %12zu | %10zu | %12.2f | %12.2f\n",
               n, sequencial.comparacoes, binaria.comparacoes,
               razao_experimental, razao_teorica);

        fprintf(csv, "%zu;%zu;%zu;%.6f;%.6f;%.6f\n",
                n, sequencial.comparacoes, binaria.comparacoes,
                razao_experimental, logaritmo, razao_teorica);
        free(vetor);
    }

    if (fclose(csv) != 0) {
        perror("Erro ao gravar o CSV");
        return EXIT_FAILURE;
    }

    printf("\nDados gravados em: %s\n", caminho_csv);
    return EXIT_SUCCESS;
}
