#ifndef CSV_READER_H
#define CSV_READER_H

#define MAX_RIVI_PITUUS 12000
#define MAX_SARAKKEET 30

// Define structure for a column
typedef struct {
    char nimi[MAX_RIVI_PITUUS];
} Sarake;

// Define structure for a row
typedef struct {
    char *arvot[MAX_SARAKKEET];
} Rivi;

Sarake *alusta_sarakkeet(const char *tiedostonimi);
Rivi **alusta_rivit(const char *tiedostonimi);
void tulosta_taulukon_tiedot(Sarake *sarakkeet, int valinta);
int laske_rivien_lkm(const char *tiedostonimi); 

#endif /* CSV_READER_H */
