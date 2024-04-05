#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RIVI_PITUUS 12000
#define MAX_SARAKE 30 // Oletetaan, että on enintään 30 saraketta

// Rakenne yksittäiselle sarakkeelle
typedef struct {
    char nimi[MAX_RIVI_PITUUS];
} Sarake;

// Rakenne yksittäiselle riville
typedef struct {
    char *arvot[MAX_SARAKE];
    Sarake sarakkeet[MAX_SARAKE]; // Sarakkeiden nimet
} Rivi;

// Funktio sarakkeiden alustamiseen
Sarake *alusta_sarakkeet(const char *tiedostonimi) {
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        fprintf(stderr, "Tiedoston avaaminen epäonnistui\n");
        exit(EXIT_FAILURE);
    }

    // Alustetaan sarakkeet-taulukko
    Sarake *sarakkeet = malloc(MAX_SARAKE * sizeof(Sarake));
    if (sarakkeet == NULL) {
        fprintf(stderr, "Muistin varaus epäonnistui\n");
        exit(EXIT_FAILURE);
    }

    int sarakkeiden_lkm = 0;

    // Siirrytään neljännelle riville
    for (int i = 0; i < 3; i++) {
        char rivi[MAX_RIVI_PITUUS];
        fgets(rivi, sizeof(rivi), tiedosto);
    }

    // Luetaan sarakkeiden nimet neljänneltä riviltä
char rivi[MAX_RIVI_PITUUS];
fgets(rivi, sizeof(rivi), tiedosto); // Luetaan rivi tiedostosta
char *token = strtok(rivi, ","); // Erota rivin osat pilkulla
while (token != NULL) {
    printf("Sarake nimi: %s\n", token); // Tulostetaan sarakkeen nimi ennen tallentamista
    strcpy(sarakkeet[sarakkeiden_lkm].nimi, token); // Tallenna sarakkeen nimi
    sarakkeiden_lkm++; // Siirry seuraavaan sarakkeeseen
    token = strtok(NULL, ","); // Hae seuraava token
}

fclose(tiedosto); // Sulje tiedosto
return sarakkeet; // Palauta alustetut sarakkeet
}

// Muokattu funktio rivin alustamiseen
Rivi *alusta_rivi(char *rivi_str, Sarake *sarakkeet) {
    Rivi *uusi_rivi = malloc(sizeof(Rivi)); // Alusta uusi rivirakenne
    if (uusi_rivi == NULL) {
        perror("Muistin varaaminen epäonnistui"); // Tulosta virheilmoitus
        exit(EXIT_FAILURE); // Lopeta ohjelma virheellä
    }

    // Erotellaan rivin arvot ja tallennetaan ne uusiin arvoihin
    char *token = strtok(rivi_str, ","); // Erota rivin osat pilkulla
    int sarake_indeksi = 0;
    while (token != NULL && sarake_indeksi < MAX_SARAKE) {
        // Tallenna arvo rivin rakenteeseen
        uusi_rivi->arvot[sarake_indeksi] = strdup(token);
        sarake_indeksi++; // Siirry seuraavaan sarakkeeseen
        token = strtok(NULL, ","); // Hae seuraava token
    }

    return uusi_rivi; // Palauta alustettu rivi
}



// Muokattu funktio rivien alustamiseen
Rivi **alusta_rivit(const char *tiedostonimi, Sarake *sarakkeet) {
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        perror("Virhe avattaessa tiedostoa");
        return NULL;
    }

    Rivi **rivit = malloc(MAX_RIVI_PITUUS * sizeof(Rivi *));
    if (rivit == NULL) {
        perror("Muistin varaaminen epäonnistui");
        fclose(tiedosto);
        return NULL;
    }

    char rivi[MAX_RIVI_PITUUS];
    int rivien_lkm = 0;

    // Ohitetaan neljä ensimmäistä riviä, joissa on sarakkeiden nimet
    for (int i = 0; i < 4; i++) {
        fgets(rivi, MAX_RIVI_PITUUS, tiedosto);
    }

    // Lue tiedostosta niin monta riviä kuin mahdollista
    while (fgets(rivi, MAX_RIVI_PITUUS, tiedosto)) {
        rivit[rivien_lkm] = alusta_rivi(rivi, sarakkeet);
        rivien_lkm++;
    }

    fclose(tiedosto);
    return rivit;
}



void tulosta_taulukon_tiedot(Sarake *sarakkeet, int valinta) {
    for (int i = 0; i < valinta; i++) {
        printf("%s\n", sarakkeet[i].nimi);
    }
}

int laske_rivien_lkm(const char *tiedostonimi) {
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        fprintf(stderr, "Tiedoston avaaminen epäonnistui\n");
        exit(EXIT_FAILURE);
    }

    int rivien_lkm = 0;
    char rivi[MAX_RIVI_PITUUS];

    while (fgets(rivi, sizeof(rivi), tiedosto) != NULL) {
        rivien_lkm++;
    }

    fclose(tiedosto);
    return rivien_lkm;
}
