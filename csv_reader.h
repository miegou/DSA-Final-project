#ifndef CSV_READER_H
#define CSV_READER_H

#define MAX_SARAKKEET 30
#define MAX_SARAKKEEN_NIMEN_PITUUS 50
#define MAX_ARVO_PITUUS 300
#define MAX_RIVI_PITUUS (MAX_SARAKKEET * MAX_ARVO_PITUUS)

// Sarake-rakenne
typedef struct {
    char nimi[MAX_SARAKKEEN_NIMEN_PITUUS];
} Sarake;

// Rivi-rakenne
typedef struct {
    char arvot[MAX_SARAKKEET][MAX_ARVO_PITUUS]; // Arvot kullekin sarakkeelle
} Rivi;

Sarake *alusta_sarakkeet(const char *tiedostonimi);
Rivi **alusta_rivit(const char *tiedostonimi, Sarake *sarakkeet);
int laske_rivien_lkm(const char *tiedostonimi);

#endif /* CSV_READER_H */
