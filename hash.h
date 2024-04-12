#ifndef HASH_H
#define HASH_H
#include "csv_reader.h"

typedef struct RiviNode {
    Rivi *rivi;
    struct RiviNode *seuraava;
} RiviNode;

typedef struct HashNode {
    char nimi[MAX_SARAKKEEN_NIMEN_PITUUS];
    RiviNode *rivit; // Linkitetty lista riveistä
    struct HashNode *seuraava;  // Lisätään linkitetyn listan solmu
} HashNode;


typedef struct HashTable {
    int koko;
    HashNode *solmut[MAX_SARAKKEET];
} HashTable;

void lisaa_sarakkeet_hajautustauluun(HashTable *ht, Sarake *sarakkeet);
void lisaa_rivi(HashTable *ht, Sarake *sarakkeet, char (*rivin_arvot)[MAX_ARVO_PITUUS]);
HashTable * luo_hajautustaulu();
void lisaa_sarake_arvo(HashTable *ht, char *sarakkeen_nimi, char *arvo);
void vapauta_hajautustaulu(HashTable *ht);
void vapauta_sarakkeen_rivit(RiviNode *rivit);
int hajautusfunktio(const char *avain);
void tulosta_hajautustaulu(HashTable *ht);
void etsi_tuotteet_sarakkeen_perusteella(HashTable *ht, char *sarakkeen_nimi, char *etsittava_arvo);

#endif
