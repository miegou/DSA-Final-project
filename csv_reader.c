#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"

// Alustaa sarakkeet tiedostosta
Sarake *alusta_sarakkeet(const char *tiedostonimi) {
    // Avataan tiedosto lukemista varten
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        perror("Tiedoston avaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Luetaan tiedostosta rivi, jossa on sarakkeiden nimet (neljäs rivi)
    char rivi[MAX_RIVI_PITUUS];
    for (int i = 0; i < 4; i++) {
        fgets(rivi, sizeof(rivi), tiedosto);
    }

    // Alustetaan taulukko sarakkeiden tallettamiseen
    Sarake *sarakkeet = malloc(MAX_SARAKKEET * sizeof(Sarake));
    if (sarakkeet == NULL) {
        perror("Muistin varaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Jaetaan rivi pilkun avulla ja tallennetaan sarakkeiden nimet taulukkoon
    char *token = strtok(rivi, ",");
    int sarakkeen_indeksi = 0;
    while (token != NULL && sarakkeen_indeksi < MAX_SARAKKEET) {
        strcpy(sarakkeet[sarakkeen_indeksi].nimi, token);
        sarakkeen_indeksi++;
        token = strtok(NULL, ",");
    }

    // Suljetaan tiedosto ja palautetaan sarakkeiden nimet
    fclose(tiedosto);
    return sarakkeet;
}

// Alustaa rivin annetusta merkkijonosta ja sarakkeista
Rivi *alusta_rivi(char *rivi_str, Sarake *sarakkeet) {
    // Alustetaan uusi Rivi-rakenne
    Rivi *uusi_rivi = malloc(sizeof(Rivi));
    if (uusi_rivi == NULL) {
        perror("Muistin varaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Jaetaan rivi pilkun avulla ja tallennetaan arvot Rivi-rakenteeseen
    char *token = strtok(rivi_str, ",");
    int sarake_indeksi = 0;
    while (token != NULL && sarake_indeksi < MAX_SARAKKEET) {
        if (strcmp(token, "") != 0) {
            strcpy(uusi_rivi->arvot[sarake_indeksi], token);
        }
        sarake_indeksi++;
        token = strtok(NULL, ",");
    }
    return uusi_rivi;
}

// Alustaa rivit annetusta tiedostosta ja sarakkeista
Rivi **alusta_rivit(const char *tiedostonimi, Sarake *sarakkeet) {
    // Avataan tiedosto lukemista varten
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        perror("Tiedoston avaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Ohita rivit 1-4
    for (int i = 0; i < 4; i++) {
        char rivi[MAX_RIVI_PITUUS];
        fgets(rivi, sizeof(rivi), tiedosto);
    }

    // Lasketaan tiedostossa olevien rivien määrä
    int rivien_lkm = laske_rivien_lkm(tiedostonimi);

    // Alustetaan taulukko rivien tallettamiseen
    Rivi **rivit = malloc(rivien_lkm * sizeof(Rivi *));
    if (rivit == NULL) {
        perror("Muistin varaaminen epäonnistui");
        fclose(tiedosto);
        exit(EXIT_FAILURE);
    }

    // Luetaan rivi kerrallaan tiedostosta ja alustetaan rivi kutsumalla alusta_rivi-funktiota
    char rivi[MAX_RIVI_PITUUS];
    int rivien_maara = 0;
    while (fgets(rivi, sizeof(rivi), tiedosto)) {
        rivit[rivien_maara] = alusta_rivi(rivi, sarakkeet);
        rivien_maara++;
    }

    // Suljetaan tiedosto ja palautetaan rivit
    fclose(tiedosto);
    return rivit;
}

// Laskee rivien määrän tiedostossa
int laske_rivien_lkm(const char *tiedostonimi) {
    // Avataan tiedosto lukemista varten
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        perror("Tiedoston avaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Lasketaan rivien määrä
    int rivien_lkm = 0;
    char rivi[MAX_RIVI_PITUUS];
    while (fgets(rivi, sizeof(rivi), tiedosto) != NULL) {
        rivien_lkm++;
    }

    // Suljetaan tiedosto ja palautetaan rivien määrä
    fclose(tiedosto);
    return rivien_lkm;
}
