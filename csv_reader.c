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

    // Ohitetaan rivit 1-4
    for (int i = 0; i < 4; i++) {
        char rivi[MAX_RIVI_PITUUS];
        fgets(rivi, sizeof(rivi), tiedosto);
    }

    // Luetaan tiedostosta rivi, jossa on sarakkeiden nimet (viides rivi)
    char rivi[MAX_RIVI_PITUUS];
    fgets(rivi, sizeof(rivi), tiedosto);

    // Alustetaan taulukko sarakkeiden tallettamiseen
    Sarake *sarakkeet = malloc(MAX_SARAKKEET * sizeof(Sarake));
    if (sarakkeet == NULL) {
        perror("Muistin varaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Jaetaan rivi ';' -merkin avulla ja tallennetaan sarakkeiden nimet taulukkoon
    char *token = strtok(rivi, ";");
    int sarakkeen_indeksi = 0;
    while (token != NULL && sarakkeen_indeksi < MAX_SARAKKEET) {
        strcpy(sarakkeet[sarakkeen_indeksi].nimi, token);
        sarakkeen_indeksi++;
        token = strtok(NULL, ";");
    }

    // Suljetaan tiedosto ja palautetaan sarakkeiden nimet
    fclose(tiedosto);
    return sarakkeet;
}

// Alustaa rivin annetusta merkkijonosta ja sarakkeista
Rivi *alusta_rivi(char *rivi_str, Sarake *sarakkeet, int sarakkeiden_maara) {
    // Alustetaan uusi Rivi-rakenne
    Rivi *uusi_rivi = malloc(sizeof(Rivi));
    if (uusi_rivi == NULL) {
        perror("Muistin varaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Käytetään strchr-funktiota pilkkujen etsimiseen ja tarkistetaan jokainen sarakkeen arvo erikseen
    int sarake_indeksi = 0;
    char *token = rivi_str;
    char *pilkku;
    while ((pilkku = strchr(token, ';')) != NULL && sarake_indeksi < sarakkeiden_maara) {
        *pilkku = '\0'; // Korvataan pilkku loppumerkillä, jotta tokeni loppuu siihen
        // Tarkistetaan, onko sarakkeen arvo tyhjä
        if (strcmp(token, "") == 0) {
            strcpy(uusi_rivi->arvot[sarake_indeksi], "ei arvoa");
        } else {
            strcpy(uusi_rivi->arvot[sarake_indeksi], token);
        }
        sarake_indeksi++;
        token = pilkku + 1; // Siirrytään seuraavaan sarakkeeseen
    }
    // Tarkistetaan vielä viimeinen sarakkeen arvo
    if (strcmp(token, "") == 0) {
        strcpy(uusi_rivi->arvot[sarake_indeksi], "ei arvoa");
    } else {
        strcpy(uusi_rivi->arvot[sarake_indeksi], token);
    }

    return uusi_rivi;
}


// Alustaa rivit annetusta tiedostosta ja sarakkeista
Rivi **alusta_rivit(const char *tiedostonimi, Sarake *sarakkeet, int *rivien_maara_ptr) {
    // Avataan tiedosto lukemista varten
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        perror("Tiedoston avaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Ohitetaan rivit 1-4
    for (int i = 0; i < 4; i++) {
        char rivi[MAX_RIVI_PITUUS];
        fgets(rivi, sizeof(rivi), tiedosto);
    }

    // Lasketaan tiedostossa olevien rivien määrä
    int rivien_lkm = 0;
    char rivi[MAX_RIVI_PITUUS];
    while (fgets(rivi, sizeof(rivi), tiedosto) != NULL) {
        rivien_lkm++;
    }

    // Siirretään tiedoston lukupää alkuun
    rewind(tiedosto);

    // Ohitetaan taas rivit 1-4
    for (int i = 0; i < 4; i++) {
        char rivi[MAX_RIVI_PITUUS];
        fgets(rivi, sizeof(rivi), tiedosto);
    }

    // Alustetaan taulukko rivien tallettamiseen
    Rivi **rivit = malloc(rivien_lkm * sizeof(Rivi *));
    if (rivit == NULL) {
        perror("Muistin varaaminen epäonnistui");
        fclose(tiedosto);
        exit(EXIT_FAILURE);
    }

    // Luetaan rivi kerrallaan tiedostosta ja alustetaan rivi kutsumalla alusta_rivi-funktiota
    int rivien_maara = 0;
    while (fgets(rivi, sizeof(rivi), tiedosto) != NULL && rivien_maara < 11270) {
        rivit[rivien_maara] = alusta_rivi(rivi, sarakkeet, MAX_SARAKKEET);
        rivien_maara++;
    }

    // Suljetaan tiedosto, päivitetään rivien määrä ja palautetaan rivit
    fclose(tiedosto);
    *rivien_maara_ptr = rivien_maara;
    return rivit;
}

void tulosta_sarakkeiden_arvot(char ***sarakkeiden_arvot, int rivien_maara, int sarakkeiden_maara) {
    printf("Sarakkeiden arvot:\n");
    for (int i = 0; i < rivien_maara; i++) {
        for (int j = 0; j < sarakkeiden_maara; j++) {
            printf("%s\t", sarakkeiden_arvot[i][j]);
        }
        printf("\n");
    }
}

// Tulostaa rivin tiedot
void tulosta_rivi(Rivi *rivi, int sarakkeiden_maara) {
    for (int i = 0; i < sarakkeiden_maara; i++) {
        printf("%s\n", rivi->arvot[i]);
    }
    printf("\n");
}

// Hakee eri arvojen määrän sarakkeessa 12 koko tiedostossa
int laske_erilaiset_arvot(Rivi **rivit, int rivien_maara, char ***taulukko) {
    // Alusta lista eri arvojen tallentamiseksi
    int max_rivit = 1000; // Alkuperäinen rivien määrä
    *taulukko = malloc(max_rivit * sizeof(char *));
    for (int i = 0; i < max_rivit; i++) {
        (*taulukko)[i] = malloc(MAX_SARAKKEET * sizeof(char));
    }
    int erilaisten_maara = 0;
    // Käy läpi rivit ja tallenna erilaiset arvot sarakkeessa 12
    for (int rivi_indeksi = 0; rivi_indeksi < rivien_maara; rivi_indeksi++) {
        char *sarake = rivit[rivi_indeksi]->arvot[12];
        int loydetty = 0;
        for (int i = 0; i < erilaisten_maara; i++) {
            if (strcmp((*taulukko)[i], sarake) == 0) {
                loydetty = 1;
                break;
            }
        }
        if (!loydetty) {
            if (erilaisten_maara >= max_rivit) { // Tarkista, onko taulukko täynnä
                max_rivit *= 2; // Kaksinkertaista taulukon koko
                *taulukko = realloc(*taulukko, max_rivit * sizeof(char *));
                for (int i = erilaisten_maara; i < max_rivit; i++) {
                    (*taulukko)[i] = malloc(MAX_SARAKKEET * sizeof(char));
                }
            }
            strncpy((*taulukko)[erilaisten_maara], sarake, MAX_SARAKKEET - 1);
            (*taulukko)[erilaisten_maara][MAX_SARAKKEET - 1] = '\0'; // Varmista, että merkkijono päättyy nolla-merkkiin
            erilaisten_maara++;
        }
    } // Add the missing closing brace for the outermost for loop

    // Tulosta lista erilaisista arvoista
    for (int i = 0; i < erilaisten_maara; i++) {
        printf("%s\n", (*taulukko)[i]);
    }

    return erilaisten_maara;
}
