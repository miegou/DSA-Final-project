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
    int otsikon_rivi_indeksi = 0;
    int arvon_indeksi_rivilla = 0;

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
    scanf("%d", &otsikon_rivi_indeksi);

    // Laske erilaisten arvojen määrä
    //int erilaisten_maara = nayta_erilaiset_arvot(rivit, rivien_maara, otsikon_rivi_indeksi);

    // Luo hajautustaulu
    HashTable *ht = luo_hajautustaulu();

    // Lisää rivit hajautustauluun
    lisaa_rivit_hajautustauluun(&ht, rivit, rivien_maara, otsikon_rivi_indeksi);

    // 
    arvot = luo_arvojnr_listasta(ht, otsikon_rivi_indeksi);

    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        if (arvot[i].arvo != NULL && arvot[i].jnr != -1) {
            printf("%d: %s\n", arvot[i].jnr, arvot[i].arvo);
        }
    }

    printf("Missä luokassa haluat jatkaa hakuasi? Valitse indeksi:\n");
    scanf("%d", &arvon_indeksi_rivilla);


    jaa_hajautustaulu_uudelleen(&ht, arvot, arvon_indeksi_rivilla, otsikon_rivi_indeksi);
    arvot = luo_arvojnr_listasta(ht, otsikon_rivi_indeksi);

    int jatka = 1; // Alustetaan jatkamismuuttuja ykköseksi
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
            scanf("%d", &otsikon_rivi_indeksi);
            
            arvot = luo_arvojnr_listasta(ht, otsikon_rivi_indeksi);

            jaa_hajautustaulu_uudelleen(&ht, arvot, arvon_indeksi_rivilla, otsikon_rivi_indeksi);

            
            printf("Missä luokassa haluat jatkaa hakuasi? Valitse indeksi:\n");
            scanf("%d", &arvon_indeksi_rivilla);
            printf("Löytyi %d tuotetta\n", laske_erilaiset_arvot(ht, arvot, otsikon_rivi_indeksi));
        } else {
            jatka = 0; // Asetetaan jatkamismuuttuja nollaksi, jos käyttäjä ei halua enää tarkentaa hakua
        }
    }

    hae_arvoa_hajautustaulusta(ht, arvot);

    // Vapauta dynaamisesti varatut muistit
    vapauta_muisti(rivit, sarakkeet);
    vapauta_hajautustaulu(ht);

    return 0;
}
