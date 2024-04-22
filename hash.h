#ifndef HASH_H
#define HASH_H
#include "csv_reader.h"

#define HASH_TAULUN_KOKO 60

extern int hajautettavan_arvon_indeksi;
extern int rivien_maara;

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

typedef struct {
    char *arvo;
    int jnr; // Järjestysnumero
} ArvoJnr;

//void lisaa_sarakkeet_hajautustauluun(HashTable *ht, Sarake *sarakkeet);
void lisaa_rivit_hajautustauluun(HashTable **ht, Rivi **rivit, int rivien_maara, int sarakkeen_indeksi);
void jaa_hajautustaulu_uudelleen(HashTable **alkuperaiset_ht, int alkuperaisen_indeksi, int uuden_indeksi);
HashTable * luo_hajautustaulu();
HashTable *luo_uusi_hajautustaulu();
//void vapauta_hajautustaulu(HashTable *ht);
//void vapauta_sarakkeen_rivit(RiviNode *rivit);
//void tulosta_hajautustaulu(HashTable *ht);
unsigned int laske_hash(char *s);
void vapauta_hajautustaulu(HashTable *ht);
void hae_arvoa_hajautustaulusta(HashTable *ht, ArvoJnr *arvot);
int laske_erilaiset_arvot(HashTable *ht, int hajautettavan_arvon_indeksi);
ArvoJnr *nayta_erilaiset_arvot_karsituista(HashTable *ht, int indeksi, int sarakkeen_indeksi);
#endif