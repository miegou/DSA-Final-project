#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"
#include "hash.h"

extern int rivien_maara;


// Alustaa sarakkeet annetusta tiedostosta
Sarake *alusta_sarakkeet(const char *tiedostonimi) {
    // Avataan tiedosto lukemista varten
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        perror("Tiedoston avaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Ohitetaan rivit 1-3
    for (int i = 0; i < 3; i++) {
        char rivi[MAX_RIVI_PITUUS];
        fgets(rivi, sizeof(rivi), tiedosto);
    }

    // Luetaan tiedostosta rivi, jossa on sarakkeiden nimet (neljäs rivi)
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


Rivi *alusta_rivi(char *rivi_str, int sarakkeiden_maara) {
    // Alustetaan uusi Rivi-rakenne
    Rivi *uusi_rivi = malloc(sizeof(Rivi));
    if (uusi_rivi == NULL) {
        perror("Muistin varaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Alustetaan jokainen arvot-taulukon jäsen osoittamaan merkkijonoon
    for (int i = 0; i < sarakkeiden_maara; i++) {
        uusi_rivi->arvot[i] = malloc(MAX_ARVO_PITUUS * sizeof(char));
        if (uusi_rivi->arvot[i] == NULL) {
            perror("Muistin varaaminen epäonnistui");
            exit(EXIT_FAILURE);
        }
    }

    // Käytetään strchr-funktiota pilkkujen etsimiseen ja tarkistetaan jokainen sarakkeen arvo erikseen
    int sarake_indeksi = 0;
    char *token = rivi_str;
    char *puolipilkku;
    while ((puolipilkku = strchr(token, ';')) != NULL && sarake_indeksi < sarakkeiden_maara) {
        *puolipilkku = '\0'; // Korvataan puolipilkku loppumerkillä, jotta tokeni loppuu siihen
        // Tarkistetaan, onko sarakkeen arvo tyhjä
        if (strcmp(token, "") == 0) {
            strcpy(uusi_rivi->arvot[sarake_indeksi], "ei arvoa");
        } else {
            strcpy(uusi_rivi->arvot[sarake_indeksi], token);
        }
        sarake_indeksi++;
        token = puolipilkku + 1; // Siirrytään seuraavaan sarakkeeseen
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
Rivi **alusta_rivit(const char *tiedostonimi, int *rivien_maara_ptr) {
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
        rivit[rivien_maara] = alusta_rivi(rivi, MAX_SARAKKEET);
        rivien_maara++;
    }

    // Suljetaan tiedosto, päivitetään rivien määrä ja palautetaan rivit
    fclose(tiedosto);
    *rivien_maara_ptr = rivien_maara;
    return rivit;
}



// Palauttaa rivin tiedot
void tulosta_rivi(Rivi *rivi, int sarakkeiden_maara) {
    for (int i = 0; i < sarakkeiden_maara; i++) {
        printf("%s\n", rivi->arvot[i]);
    }
    
}

int nayta_erilaiset_arvot(Rivi **rivit, int rivien_maara, int sarakkeen_indeksi) {
    int erilaisten_maara = 0;

    // Käy läpi rivit ja tallenna erilaiset arvot pyydetyssä sarakkeessa
    for (int rivi_indeksi = 0; rivi_indeksi < rivien_maara; rivi_indeksi++) {
        char *sarake = rivit[rivi_indeksi]->arvot[sarakkeen_indeksi];
        int loydetty = 0;
        // Tarkista, onko arvo jo olemassa
        for (int i = 0; i < erilaisten_maara; i++) {
            if (strcmp(rivit[i]->arvot[sarakkeen_indeksi], sarake) == 0) {
                loydetty = 1;
                break;
            }
        }
        // Jos arvoa ei löydy, lisää se listaan
        if (!loydetty) {
            strcpy(rivit[erilaisten_maara]->arvot[sarakkeen_indeksi], sarake);
            erilaisten_maara++;
        }
    }
    // //Tulosta lista erilaisista arvoista
    for (int i = 0; i < erilaisten_maara; i++) {
        printf("%d. %s\n", laske_hash(rivit[i]->arvot[sarakkeen_indeksi]), rivit[i]->arvot[sarakkeen_indeksi]);
    }

    return erilaisten_maara;
}


// Vapauttaa dynaamisesti varatun muistin
void vapauta_muisti(Rivi **rivit, Sarake *sarakkeet) {
    free(rivit);
    free(sarakkeet);
}