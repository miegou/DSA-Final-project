#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RIVI_PITUUS 1024
#define MAX_SARAKE 100

void lue_csv_tiedosto(const char *tiedostonimi) {
    FILE *tiedosto = fopen(tiedostonimi, "r");
    if (tiedosto == NULL) {
        perror("Virhe avattaessa tiedostoa");
        return;
    }

    char rivi[MAX_RIVI_PITUUS];
    while (fgets(rivi, MAX_RIVI_PITUUS, tiedosto) != NULL) {
        char *token;
        char *sarake[MAX_SARAKE];
        int sarakkeiden_lkm = 0;

        token = strtok(rivi, ",");
        while (token != NULL && sarakkeiden_lkm < MAX_SARAKE) {
            sarake[sarakkeiden_lkm++] = token;
            token = strtok(NULL, ",");
        }

        // Tulosta sarakkeet
        for (int i = 0; i < sarakkeiden_lkm; i++) {
            printf("%s\t", sarake[i]);
        }
        printf("\n");
    }

    fclose(tiedosto);
}

int main() {
    const char *tiedostonimi = "alkon-hinnasto-tekstitiedostona.csv";  // Korvaa "tiedosto.csv" omalla tiedostosi nimellä
    lue_csv_tiedosto(tiedostonimi);
    return 0;
}
