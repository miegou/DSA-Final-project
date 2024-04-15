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
    char arvot[MAX_SARAKKEET][MAX_ARVO_PITUUS]; // Arvot kullekin sarakkeelle
} Rivi;

Sarake *alusta_sarakkeet(const char *tiedostonimi);
Rivi **alusta_rivit(const char *tiedostonimi, Sarake *sarakkeet, int *rivien_maara_ptr);
int laske_rivien_lkm(const char *tiedostonimi);
char ***hae_sarakkeiden_arvot(const char *tiedostonimi, int *rivien_maara_ptr, int *sarakkeiden_maara_ptr);
void tulosta_sarakkeiden_arvot(char ***sarakkeiden_arvot, int rivien_maara, int sarakkeiden_maara);
void tulosta_rivi(Rivi *rivi, int sarakkeiden_maara);
int laske_erilaiset_arvot(Rivi **rivit, int rivien_maara, char ***taulukko);

#endif /* CSV_READER_H */
