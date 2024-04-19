#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"
#include "hash.h"

#define TIEDOSTONIMI "alkon-hinnasto-tekstitiedostona-muokattu.csv"

int main() {
    int rivien_maara = 0;
    const char *tiedostonimi = TIEDOSTONIMI;
    Rivi **rivit = NULL;
    int hajautettavan_arvon_indeksi = 0;

    // Alusta sarakkeet ja rivit
    Sarake *sarakkeet = alusta_sarakkeet(tiedostonimi);
    rivit = alusta_rivit(tiedostonimi, &rivien_maara);
    printf("Rivejä: %d\n", rivien_maara);

    // Tulosta sarakkeet
    for (int i = 0; i < MAX_SARAKKEET; i++) {
        printf("%d. %s\n", i, sarakkeet[i].nimi);
    }
    //Kysytään käyttäjältä sarakkeen indeksi
    printf("Millä kategorialla haluat rajata hakuasi? Valitse sarakkeen indeksi:\n");
    scanf("%d", &hajautettavan_arvon_indeksi);

    // Laske erilaisten arvojen määrä
    //int erilaisten_maara = nayta_erilaiset_arvot(rivit, rivien_maara, hajautettavan_arvon_indeksi);

    // Luo hajautustaulu
    HashTable *ht = luo_hajautustaulu();

    // Lisää rivit hajautustauluun
    lisaa_rivit_hajautustauluun(&ht, rivit, rivien_maara, hajautettavan_arvon_indeksi);

    //Tulosta mahdolliset luokat
    nayta_erilaiset_arvot(rivit, rivien_maara, hajautettavan_arvon_indeksi);
    printf("Valitse luokka, josta haluat nähdä tuotteet:\n");
    scanf("%d", &hajautettavan_arvon_indeksi);
    hae_arvoa_hajautustaulusta(ht->hash_arvot[hajautettavan_arvon_indeksi]);


    // Vapauta dynaamisesti varatut muistit
    vapauta_muisti(rivit, sarakkeet);
    vapauta_hajautustaulu(ht);

    return 0;
}
