#include "buscas.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static const size_t TAMANHOS[] = {10, 50, 100, 500, 1000, 5000};

/* Le um numero inteiro, rejeitando letras e valores invalidos. */
static int ler_inteiro(const char *pergunta, int *valor) {
    char linha[128];
    char *fim;
    long numero;

    printf("%s", pergunta);
    fflush(stdout);

    if (fgets(linha, sizeof(linha), stdin) == NULL) {
        return 0;
    }

    /* Descarta a sobra de linhas muito longas. */
    if (linha[0] != '\0') {
        size_t i = 0;
        while (linha[i] != '\0' && linha[i] != '\n') {
            ++i;
        }
        if (linha[i] != '\n' && !feof(stdin)) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                /* Ignora o restante da entrada. */
            }
            return -1;
        }
    }

    errno = 0;
    numero = strtol(linha, &fim, 10);
    if (errno != 0 || fim == linha || numero < INT_MIN || numero > INT_MAX) {
        return -1;
    }

    while (isspace((unsigned char)*fim)) {
        ++fim;
    }
    if (*fim != '\0') {
        return -1;
    }

    *valor = (int)numero;
    return 1;
}

/* Permite selecionar pela opcao de 1 a 6 ou pelo tamanho exato. */
static int escolher_tamanho(size_t *tamanho) {
    int escolha;
    int leitura;

    puts("\nEscolha o tamanho do vetor:");
    for (size_t i = 0; i < sizeof(TAMANHOS) / sizeof(TAMANHOS[0]); ++i) {
        printf("  %zu) %zu elementos\n", i + 1, TAMANHOS[i]);
    }

    for (;;) {
        leitura = ler_inteiro("\nOpcao (1 a 6 ou tamanho exato): ", &escolha);
        if (leitura == 0) {
            return 0;
        }
        if (leitura < 0) {
            puts("Entrada invalida. Digite um numero inteiro.");
            continue;
        }
        if (escolha >= 1 && escolha <= 6) {
            *tamanho = TAMANHOS[(size_t)escolha - 1];
            return 1;
        }
        for (size_t i = 0; i < sizeof(TAMANHOS) / sizeof(TAMANHOS[0]); ++i) {
            if (escolha == (int)TAMANHOS[i]) {
                *tamanho = TAMANHOS[i];
                return 1;
            }
        }
        puts("Tamanho invalido. Escolha 10, 50, 100, 500, 1000 ou 5000.");
    }
}

/* Mostra se o alvo foi encontrado e quantas comparacoes foram feitas. */
static void exibir_resultado(const char *nome, ResultadoBusca resultado) {
    printf("\n%s\n", nome);
    if (resultado.indice >= 0) {
        printf("  Encontrado! Indice: %td (posicao %td).\n",
               resultado.indice, resultado.indice + 1);
    } else {
        puts("  Numero nao encontrado.");
    }
    printf("  Comparacoes: %zu\n", resultado.comparacoes);
}

int main(void) {
    size_t tamanho;
    int alvo;
    int *vetor;
    ResultadoBusca sequencial, binaria;

    puts("==============================================");
    puts("    BUSCA SEQUENCIAL VS. BUSCA BINARIA");
    puts("==============================================");

    if (!escolher_tamanho(&tamanho)) {
        fputs("Entrada encerrada antes da escolha do tamanho.\n", stderr);
        return EXIT_FAILURE;
    }

    for (;;) {
        int leitura = ler_inteiro("Numero que deseja procurar: ", &alvo);
        if (leitura == 0) {
            fputs("Entrada encerrada antes da escolha do numero.\n", stderr);
            return EXIT_FAILURE;
        }
        if (leitura == 1) {
            break;
        }
        puts("Numero invalido. Digite um inteiro, como 12, 21 ou -4.");
    }

    vetor = malloc(tamanho * sizeof(*vetor));
    if (vetor == NULL) {
        fputs("Nao foi possivel alocar o vetor.\n", stderr);
        return EXIT_FAILURE;
    }

    /* Vetor crescente com valores 0, 2, 4, 6, ..., 2*(n-1). */
    for (size_t i = 0; i < tamanho; ++i) {
        vetor[i] = (int)(2 * i);
    }

    sequencial = busca_sequencial(vetor, tamanho, alvo);
    binaria = busca_binaria(vetor, tamanho, alvo);

    printf("\nVetor: %zu elementos ordenados (0, 2, 4, ..., %d).\n",
           tamanho, vetor[tamanho - 1]);
    printf("Valor procurado: %d\n", alvo);
    exibir_resultado("BUSCA SEQUENCIAL", sequencial);
    exibir_resultado("BUSCA BINARIA", binaria);

    if (sequencial.indice == -1 && binaria.indice != -1) {
        fputs("Erro: as buscas discordam do resultado.\n", stderr);
        free(vetor);
        return EXIT_FAILURE;
    }
    if (sequencial.indice != -1 && binaria.indice == -1) {
        fputs("Erro: as buscas discordam do resultado.\n", stderr);
        free(vetor);
        return EXIT_FAILURE;
    }

    printf("\nRazao experimental (sequencial / binaria): %.2f\n",
           (double)sequencial.comparacoes / (double)binaria.comparacoes);
    printf("Razao teorica de crescimento (n / log2(n)): %.2f\n",
           (double)tamanho / log2((double)tamanho));
    puts("Observacao: o numero de comparacoes depende do alvo escolhido.");

    /* Armazena a busca escolhida sem substituir o CSV do experimento geral. */
    FILE *csv = fopen("resultado_busca.csv", "w");
    if (csv != NULL) {
        fprintf(csv, "n;alvo;encontrado;indice;comparacoes_sequencial;"
                     "comparacoes_binaria;razao_experimental\n");
        fprintf(csv, "%zu;%d;%d;%td;%zu;%zu;%.6f\n",
                tamanho, alvo, sequencial.indice >= 0, sequencial.indice,
                sequencial.comparacoes, binaria.comparacoes,
                (double)sequencial.comparacoes / (double)binaria.comparacoes);
        if (fclose(csv) == 0) {
            puts("\nResultado gravado em: resultado_busca.csv");
        } else {
            fputs("Aviso: falha ao finalizar resultado_busca.csv.\n", stderr);
        }
    } else {
        fputs("Aviso: nao foi possivel salvar resultado_busca.csv.\n", stderr);
    }

    free(vetor);
    return EXIT_SUCCESS;
}
