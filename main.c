#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "avl.h"
#include "csv_reader.h"
#include "hash.h"

#define TIEDOSTONIMI "alkon-hinnasto-tekstitiedostona.csv"

int main() {
    const char *tiedostonimi = TIEDOSTONIMI;
    Sarake *sarakkeet = alusta_sarakkeet(tiedostonimi);
    Rivi **rivit = alusta_rivit(tiedostonimi, sarakkeet);
    struct Node *root = NULL;
    int rivien_lkm = laske_rivien_lkm(tiedostonimi);

    // printf("Sarakkeiden nimet:\n");
    // for (int i = 0; i < MAX_SARAKKEET; i++) {
    //     printf("%d. %s\n", i + 1, sarakkeet[i].nimi);
    // }

    // Luo hajautustaulu sarakkeilla
    HashTable *ht = luo_hajautustaulu();

    lisaa_sarakkeet_hajautustauluun(ht, sarakkeet);
    laske_rivien_lkm(tiedostonimi);
    printf("Rivien lkm: %d\n", rivien_lkm);

    for (int i = 0; i < 6; i++) {
        lisaa_rivi(ht, sarakkeet, rivit[i]->arvot);
    }
    printf("Rivit lisätty hajautustauluun\n");

    char *etsittava_sarakkeen_nimi = "Hinta";
    char *etsittava_arvo = "10.48"; // Voit muuttaa tähän haluamasi hinnan
    etsi_tuotteet_sarakkeen_perusteella(ht, etsittava_sarakkeen_nimi, etsittava_arvo);
    printf("Palattiin mainiin etsi_tuotteet_sarakkeen_perusteella-funktiosta\n");

    //vapauta_hajautustaulu(ht);
    free(sarakkeet);

    return 0;
}
