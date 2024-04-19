#ifndef CSV_READER_H
#define CSV_READER_H

#define MAX_SARAKKEET 30
#define MAX_SARAKKEEN_NIMEN_PITUUS 50
#define MAX_ARVO_PITUUS 300
#define MAX_RIVI_PITUUS (MAX_SARAKKEET * MAX_ARVO_PITUUS)

#define MAX_VALUES 1000

// Sarake-rakenne
typedef struct {
    char nimi[MAX_SARAKKEEN_NIMEN_PITUUS];
} Sarake;

// Rivi-rakenne
typedef struct {
    char *arvot[MAX_ARVO_PITUUS]; // Arvot kullekin sarakkeelle
} Rivi;

Sarake *alusta_sarakkeet(const char *tiedostonimi);
Rivi **alusta_rivit(const char *tiedostonimi, int *rivien_maara_ptr);
int laske_rivien_lkm(const char *tiedostonimi);
void tulosta_rivi(Rivi *rivi, int sarakkeiden_maara);
int nayta_erilaiset_arvot(Rivi **rivit, int rivien_maara, int sarakkeen_indeksi);
void vapauta_muisti(Rivi **rivit, Sarake *sarakkeet);

#endif /* CSV_READER_H */
