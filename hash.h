#ifndef HASH_H
#define HASH_H
#include "csv_reader.h"

#define HASH_TAULUN_KOKO 120

extern int hajautettavan_arvon_indeksi;

//Määrittele RiviNode-rakenne
typedef struct RiviNode {
    Rivi *rivi; // Rivi-olio solmussa
    char *nimi; // Solmun nimi
    struct RiviNode *next; // Seuraavan solmun osoitin
} RiviNode;

// Määrittele hajautustaulun rakenne
typedef struct HashTable {
    RiviNode *hash_arvot[HASH_TAULUN_KOKO]; // Taulukko linkitettyjen listojen pääsolmuille
} HashTable;

//void lisaa_sarakkeet_hajautustauluun(HashTable *ht, Sarake *sarakkeet);
void lisaa_rivit_hajautustauluun(HashTable **ht, Rivi **rivit, int rivien_maara, int sarakkeen_indeksi);
HashTable * luo_hajautustaulu();
//void vapauta_hajautustaulu(HashTable *ht);
//void vapauta_sarakkeen_rivit(RiviNode *rivit);
//void tulosta_hajautustaulu(HashTable *ht);
unsigned int laske_hash(char *s);
void vapauta_hajautustaulu(HashTable *ht);
void hae_arvoa_hajautustaulusta(RiviNode *node);

#endif