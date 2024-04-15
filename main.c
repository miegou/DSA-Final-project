#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "avl.h"
#include "csv_reader.h"
#include "hash.h"

#define TIEDOSTONIMI "alkon-hinnasto-tekstitiedostona-muokattu.csv"
#define MAX_RIVIT 12000

int main() {
int rivien_maara = 100;
    const char *tiedostonimi = TIEDOSTONIMI;
    Rivi **rivit = NULL;
    char ***taulukko = malloc(MAX_RIVIT * sizeof(char **));
    for (int i = 0; i < MAX_RIVIT; i++) {
        taulukko[i] = malloc(MAX_SARAKKEET * sizeof(char *));
        for (int j = 0; j < MAX_SARAKKEET; j++) {
            taulukko[i][j] = malloc(MAX_ARVO_PITUUS * sizeof(char));
        }
    }

    Sarake *sarakkeet = alusta_sarakkeet(tiedostonimi);
    rivit = alusta_rivit(tiedostonimi, sarakkeet, &rivien_maara);
    printf("Rivejä: %d\n", rivien_maara);

    int erilaisten_maara = laske_erilaiset_arvot(rivit, rivien_maara, taulukko);
    printf("Eri arvoja sarakkeessa 12: %d\n", erilaisten_maara);

    // // Tulostaa 10 ensimmäistä riviä indeksistä 12
    // for (int j = 0; j < rivien_maara && j < 100; j++) {
    //     printf("%s\n", rivit[j]->arvot[12]);
    // }
    // printf("\n");

    // Vapauta muisti
    for (int i = 0; i < MAX_RIVIT; i++) {
        for (int j = 0; j < MAX_SARAKKEET; j++) {
            free(taulukko[i][j]);
        }
        free(taulukko[i]);
    }
    free(taulukko);

    // Vapauta rivit
    for (int i = 0; i < rivien_maara; i++) {
        free(rivit[i]);
    }
    free(rivit);

    return 0;
}