#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"

Sarake *alusta_sarakkeet(const char *tiedostonimi) {
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        fprintf(stderr, "Tiedoston avaaminen epäonnistui\n");
        exit(EXIT_FAILURE);
    }

    // Alustetaan sarakkeet-taulukko
    Sarake *sarakkeet = malloc(MAX_SARAKKEET * sizeof(Sarake));
    if (sarakkeet == NULL) {
        fprintf(stderr, "Muistin varaus epäonnistui\n");
        exit(EXIT_FAILURE);
    }

    int sarakkeen_indeksi = 0;

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
        strcpy(sarakkeet[sarakkeen_indeksi].nimi, token); // Tallenna sarakkeen nimi
        sarakkeen_indeksi++; // Siirry seuraavaan sarakkeeseen
        token = strtok(NULL, ","); // Hae seuraava token
    }

    fclose(tiedosto); // Sulje tiedosto
    return sarakkeet; // Palauta alustetut sarakkeet
}


#include <stdbool.h>

// Function to initialize a row
Rivi *alusta_rivi(char *rivi_str, Sarake *sarakkeet) {
    Rivi *uusi_rivi = malloc(sizeof(Rivi)); // Allocate memory for the new row
    if (uusi_rivi == NULL) {
        perror("Memory allocation failed"); // Print error message
        exit(EXIT_FAILURE); // Exit the program with failure status
    }

    // Initialize values to default (e.g., empty string or "not available")
    for (int i = 0; i < MAX_SARAKKEET; ++i) {
        uusi_rivi->arvot[i] = strdup(""); // Default value is empty string
    }

    // Tokenize the input string to get values for each column
    char *token = strtok(rivi_str, ",");
    int sarake_indeksi = 0;
    while (token != NULL && sarake_indeksi < MAX_SARAKKEET) {
        // Store the value in the row structure
        if (strcmp(token, "") != 0) { // If the token is not empty
            // Check if the token contains multiple values inside quotes
            char *quote_start = strchr(token, '"');
            char *quote_end = strrchr(token, '"');
            if (quote_start != NULL && quote_end != NULL && quote_start != quote_end) {
                // Token contains multiple values inside quotes
                // Copy the token value as is
                uusi_rivi->arvot[sarake_indeksi] = strdup(token);
            } else {
                // Token does not contain multiple values inside quotes
                uusi_rivi->arvot[sarake_indeksi] = strdup(token);
            }
        }
        sarake_indeksi++; // Move to the next column
        token = strtok(NULL, ","); // Get the next token
    }

    return uusi_rivi; // Return the initialized row
}


Rivi **alusta_rivit(const char *tiedostonimi, Sarake *sarakkeet) {
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        perror("Virhe avattaessa tiedostoa");
        return NULL;
    }

    int rivien_lkm = laske_rivien_lkm(tiedostonimi); // Laske tiedostossa olevien rivien määrä
    Rivi **rivit = malloc(rivien_lkm * sizeof(Rivi *)); // Varaa tilaa rivien lukumäärälle
    if (rivit == NULL) {
        perror("Muistin varaaminen epäonnistui");
        fclose(tiedosto);
        return NULL;
    }

    char rivi[MAX_RIVI_PITUUS];

    // Ohitetaan neljä ensimmäistä riviä, joissa on sarakkeiden nimet
    for (int i = 0; i < 4; i++) {
        fgets(rivi, MAX_RIVI_PITUUS, tiedosto);
    }

    // Lue tiedostosta niin monta riviä kuin mahdollista
    int rivien_maara = 0;
    while (fgets(rivi, MAX_RIVI_PITUUS, tiedosto)) {
        rivit[rivien_maara] = alusta_rivi(rivi, sarakkeet);
        rivien_maara++;
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
