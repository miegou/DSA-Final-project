#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RIVI_PITUUS 1024
#define MAX_SARAKE 30 // Oletetaan, että on enintään 30 saraketta

// Rakenne yksittäiselle sarakkeelle
typedef struct {
    char nimi[MAX_RIVI_PITUUS];
    char **arvot;
    int arvojen_maara;
} Sarake;

// Funktio sarakkeiden alustamiseen
Sarake *alusta_sarakkeet(const char *tiedostonimi) {
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        perror("Virhe avattaessa tiedostoa");
        return NULL;
    }

    char rivi[MAX_RIVI_PITUUS];
    Sarake *sarakkeet = malloc(MAX_SARAKE * sizeof(Sarake));
    if (sarakkeet == NULL) {
        perror("Muistin varaaminen epäonnistui");
        fclose(tiedosto);
        return NULL;
    }

    // Hyppäämme ensimmäiset 3 riviä
    for (int i = 0; i < 3; ++i) {
        fgets(rivi, MAX_RIVI_PITUUS, tiedosto);
    }

    // Luetaan neljäs rivi ja alustetaan taulukot
    if (fgets(rivi, MAX_RIVI_PITUUS, tiedosto) != NULL) {
        char *token;
        int sarakkeiden_lkm = 0;

        // Erotetaan sarakkeiden nimet ja luodaan niihin liittyvät taulukot
        token = strtok(rivi, ",");
        while (token != NULL && sarakkeiden_lkm < MAX_SARAKE) {
            strcpy(sarakkeet[sarakkeiden_lkm].nimi, token);
            sarakkeet[sarakkeiden_lkm].arvot = malloc(sizeof(char*)); // Alustetaan arvot taulukko
            sarakkeet[sarakkeiden_lkm].arvojen_maara = 0;
            sarakkeiden_lkm++;
            token = strtok(NULL, ",");
        }
    } else {
        perror("Tyhjä tiedosto tai lukuvirhe");
        fclose(tiedosto);
        free(sarakkeet);
        return NULL;
    }

    // Tallennetaan arvot taulukkoon
    while (fgets(rivi, MAX_RIVI_PITUUS, tiedosto) != NULL) {
        char *token;
        int sarakkeiden_lkm = 0;

        token = strtok(rivi, ",");
        while (token != NULL && sarakkeiden_lkm < MAX_SARAKE) {
            sarakkeet[sarakkeiden_lkm].arvot = realloc(sarakkeet[sarakkeiden_lkm].arvot, (sarakkeet[sarakkeiden_lkm].arvojen_maara + 1) * sizeof(char*));
            sarakkeet[sarakkeiden_lkm].arvot[sarakkeet[sarakkeiden_lkm].arvojen_maara] = malloc((strlen(token) + 1) * sizeof(char));
            strcpy(sarakkeet[sarakkeiden_lkm].arvot[sarakkeet[sarakkeiden_lkm].arvojen_maara], token);
            sarakkeet[sarakkeiden_lkm].arvojen_maara++;
            sarakkeiden_lkm++;
            token = strtok(NULL, ",");
        }
    }

    fclose(tiedosto);
    return sarakkeet;
}

// Funktio tulostaa taulukon tiedot
void tulosta_taulukon_tiedot(Sarake *sarakkeet, int valinta) {
    if (valinta < 1 || valinta > MAX_SARAKE || strlen(sarakkeet[valinta - 1].nimi) == 0) {
        printf("Virheellinen valinta.\n");
        return;
    }

    printf("Taulukon %d tiedot (%s):\n", valinta, sarakkeet[valinta - 1].nimi);
    for (int i = 0; i < sarakkeet[valinta - 1].arvojen_maara; i++) {
        printf("%s\n", sarakkeet[valinta - 1].arvot[i]);
    }
}

int main() {
    const char *tiedostonimi = "alkon-hinnasto-tekstitiedostona.csv";  // Korvaa "tiedosto.csv" omalla tiedostosi nimellä
    Sarake *sarakkeet = alusta_sarakkeet(tiedostonimi);
    if (sarakkeet == NULL) {
        printf("Sarakkeiden alustaminen epäonnistui\n");
        return 1;
    }

    // Tulosta sarakkeiden numerot ja nimet
    printf("Sarakkeiden numerot ja nimet:\n");
    for (int i = 0; i < MAX_SARAKE; i++) {
        if (strlen(sarakkeet[i].nimi) > 0) {
            printf("%d. %s\n", i + 1, sarakkeet[i].nimi);
        }
    }

    // Kysy käyttäjältä taulukon numero
    int valinta;
    printf("Valitse taulukko (1-%d): ", MAX_SARAKE);
    scanf("%d", &valinta);

    // Tulosta valitun taulukon tiedot
    tulosta_taulukon_tiedot(sarakkeet, valinta);

    // Vapauta dynaamisesti varattu muisti
    for (int i = 0; i < MAX_SARAKE; i++) {
        for (int j = 0; j < sarakkeet[i].arvojen_maara; j++) {
            free(sarakkeet[i].arvot[j]);
        }
        free(sarakkeet[i].arvot);
    }
    free(sarakkeet);

    return 0;
}
