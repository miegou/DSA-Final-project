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
    ArvoJnr *arvot;
    int hajautettavan_arvon_indeksi = 0;
    int uuden_indeksi = 0;

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
    printf("Löytyi %d erilaista luokkaa\n", nayta_erilaiset_arvot(rivit, rivien_maara, hajautettavan_arvon_indeksi));
    printf("Missä luokassa haluat jatkaa hakuasi? Valitse indeksi:\n");
    scanf("%d", &hajautettavan_arvon_indeksi);
    printf("Löytyi %d tuotetta\n", laske_erilaiset_arvot(ht, hajautettavan_arvon_indeksi));
    int jatka = 1; // Alustetaan jatkamismuuttuja

    while (jatka) {
        printf("Haluatko tarkentaa hakua? (1 = kyllä, 0 = ei)\n");
        int valinta;
        scanf("%d", &valinta); 
        if (valinta == 1) {
            // Tulosta sarakkeet
            for (int i = 0; i < MAX_SARAKKEET; i++) {
                printf("%d. %s\n", i, sarakkeet[i].nimi);
            }
            printf("Millä kategorialla haluat rajata seuraavaksi, valitse indeksi:\n");
            int uuden_indeksi;
            scanf("%d", &uuden_indeksi);
            arvot = nayta_erilaiset_arvot_karsituista(ht, hajautettavan_arvon_indeksi, uuden_indeksi);
            //printf("Löytyi %d tuotetta\n", laske_erilaiset_arvot(ht, uuden_indeksi));
            jaa_hajautustaulu_uudelleen(&ht, hajautettavan_arvon_indeksi, uuden_indeksi);
            //printf("Löytyi %d erilaista luokkaa\n", nayta_erilaiset_arvot_karsituista(ht, hajautettavan_arvon_indeksi, uuden_indeksi, &erilaisten_maara));
        } else {
            jatka = 0; // Asetetaan jatkamismuuttuja nollaksi, jos käyttäjä ei halua enää tarkentaa hakua
        }
    }

    // Hae arvoa hajautustaulusta
    //nayta_erilaiset_arvot_karsituista(ht, hajautettavan_arvon_indeksi, uuden_indeksi, &erilaisten_maara);
    // printf("Millä arvolla haluat nähdä tuotteet?\n");
    // printf("Anna indeksi:\n");
    // scanf("%d", &uuden_indeksi);
    hae_arvoa_hajautustaulusta(ht, arvot);


    // Vapauta dynaamisesti varatut muistit
    vapauta_muisti(rivit, sarakkeet);
    vapauta_hajautustaulu(ht);

    return 0;
}
