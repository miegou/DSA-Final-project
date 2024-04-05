#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RIVI_PITUUS 12000
#define MAX_SARAKE 30 // Oletetaan, että on enintään 30 saraketta

// Rakenne yksittäiselle riville
typedef struct {
    char *arvot[MAX_SARAKE];
    int sarake_lkm;
} Rivi;

// Funktio rivin alustamiseen
Rivi *alusta_rivi() {
    Rivi *rivi = malloc(sizeof(Rivi));
    if (rivi == NULL) {
        perror("Muistin varaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }
    rivi->sarake_lkm = 0;
    return rivi;
}

// Funktio vapauttaa rivin muistin
void vapauta_rivi(Rivi *rivi) {
    for (int i = 0; i < rivi->sarake_lkm; i++) {
        free(rivi->arvot[i]);
    }
    free(rivi);
}

// Funktio sarakkeiden alustamiseen
Rivi **alusta_sarakkeet(const char *tiedostonimi) {
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        perror("Virhe avattaessa tiedostoa");
        return NULL;
    }

    char rivi[MAX_RIVI_PITUUS];
    Rivi **sarakkeet = malloc(MAX_RIVI_PITUUS * sizeof(Rivi*));
    if (sarakkeet == NULL) {
        perror("Muistin varaaminen epäonnistui");
        fclose(tiedosto);
        return NULL;
    }

    // Luetaan rivit tiedostosta
    int rivi_indeksi = 0;
    while (fgets(rivi, MAX_RIVI_PITUUS, tiedosto) != NULL && rivi_indeksi < MAX_RIVI_PITUUS) {
        Rivi *uusi_rivi = alusta_rivi();

        // Erotellaan rivin arvot sarakkeiksi ja tallennetaan ne riviin
        char *token = strtok(rivi, ",");
        int sarake_indeksi = 0;
        while (token != NULL && sarake_indeksi < MAX_SARAKE) {
            uusi_rivi->arvot[sarake_indeksi] = strdup(token); // Kopioidaan arvo ja varataan sille muisti
            sarake_indeksi++;
            token = strtok(NULL, ",");
        }
        uusi_rivi->sarake_lkm = sarake_indeksi;

        // Tallennetaan rivi sarakkeisiin
        sarakkeet[rivi_indeksi] = uusi_rivi;
        rivi_indeksi++;
    }

    fclose(tiedosto);
    return sarakkeet;
}

// Funktio tulostaa taulukon tiedot
void tulosta_taulukon_tiedot(Rivi **sarakkeet, int rivi_maara, int valinta) {
    if (valinta < 1 || valinta > rivi_maara || valinta > MAX_RIVI_PITUUS) {
        printf("Virheellinen valinta.\n");
        return;
    }

    printf("Taulukon %d. rivi tiedot:\n", valinta);
    Rivi *rivi = sarakkeet[valinta - 1];
    for (int i = 0; i < rivi->sarake_lkm; i++) {
        printf("%s\n", rivi->arvot[i]);
    }
}

int main() {
    const char *tiedostonimi = "alkon-hinnasto-tekstitiedostona.csv";  // Korvaa "tiedosto.csv" omalla tiedostosi nimellä
    Rivi **sarakkeet = alusta_sarakkeet(tiedostonimi);
    if (sarakkeet == NULL) {
        printf("Sarakkeiden alustaminen epäonnistui\n");
        return 1;
    }

    // Tulosta sarakkeiden numerot
    int rivi_maara = 0;
    for (int i = 0; sarakkeet[i] != NULL; i++) {
        rivi_maara++;
    }
    printf("Taulukon rivien määrä: %d\n", rivi_maara);

    // Kysy käyttäjältä rivin numeron
    int valinta;
    printf("Valitse rivi (1-%d): ", rivi_maara);
    scanf("%d", &valinta);

    // Tulosta valitun rivin tiedot
    tulosta_taulukon_tiedot(sarakkeet, rivi_maara, valinta);

    // Vapauta dynaamisesti varattu muisti
    for (int i = 0; i < rivi_maara; i++) {
        vapauta_rivi(sarakkeet[i]);
    }
    free(sarakkeet);

    return 0;
}
