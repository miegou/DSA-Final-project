#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"
#include "hash.h"

// Funktio hajautustaulun luomiseen
HashTable *luo_hajautustaulu() {
    HashTable *ht = malloc(sizeof(HashTable));
    if (ht == NULL) {
        perror("Muistin varaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Alusta hajautustaulun arvot
    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        ht->hash_arvot[i] = 0; // Voit alustaa arvot haluamallasi tavalla
    }
    printf("Hajautustaulu luotu\n");
    return ht;
}


unsigned int laske_hash(char *s) {
   unsigned hash = 0;

    for(; *s; ++s)
    {
        hash += *s;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    hash = hash % HASH_TAULUN_KOKO;

    return hash;
}


// Käy läpi rivit ja hajauttaa ne listaan annetun sarakkeen perusteella 
void lisaa_rivit_hajautustauluun(HashTable **ht, Rivi **rivit, int rivien_maara, int sarakkeen_indeksi) {
    for (int rivi_indeksi = 0; rivi_indeksi < rivien_maara; rivi_indeksi++) {
        char *nimi = rivit[rivi_indeksi]->arvot[sarakkeen_indeksi];
        int hash = laske_hash(nimi) % HASH_TAULUN_KOKO; // Laske hash-arvo ja moduloi taulukon koon kanssa
        
        // Luodaan uusi RiviNode ja lisätään se listaan
        RiviNode *new_node = malloc(sizeof(RiviNode));
        if (new_node == NULL) {
            perror("Muistin varaus epäonnistui");
            exit(EXIT_FAILURE);
        }
        new_node->rivi = rivit[rivi_indeksi];
        new_node->next = NULL;

        // Asetetaan uuden solmun nimeksi sarakkeen indeksin arvo
        new_node->nimi = strdup(nimi);

        // Tarkista, onko hash-arvon kohdalla jo lista
        if ((*ht)->hash_arvot[hash] == NULL) {
            // Jos hash-arvon kohdalla ei ole vielä listaa, luodaan uusi lista ja lisätään sinne uusi solmu
            (*ht)->hash_arvot[hash] = new_node;
        } else {
            // Jos hash-arvon kohdalla on jo lista, tarkista ensimmäisen solmun nimi
            RiviNode *current = (*ht)->hash_arvot[hash];
            if (strcmp(current->nimi, nimi) == 0) {
                // Jos ensimmäisen solmun nimi on sama kuin uuden solmun nimi, lisää solmu listaan
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = new_node;
            } else {
                // Jos ensimmäisen solmun nimi ei ole sama kuin uuden solmun nimi, käytä avointa hajautusta
                int index = (hash + 1) % HASH_TAULUN_KOKO;
                while ((*ht)->hash_arvot[index] != NULL && index != hash) {
                    index = (index + 1) % HASH_TAULUN_KOKO;
                }
                // Jos löydettiin vapaa indeksi, lisätään solmu siihen
                if ((*ht)->hash_arvot[index] == NULL) {
                    (*ht)->hash_arvot[index] = new_node;
                } else {
                    // Jos hajautustaulu on täynnä, kaadu ohjelmaan
                    fprintf(stderr, "Hajautustaulu on täynnä.\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

void vapauta_hajautustaulu(HashTable *ht) {
    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        RiviNode *current = ht->hash_arvot[i];
        while (current != NULL) {
            RiviNode *temp = current;
            current = current->next;
            free(temp->nimi); // Vapauta nimi-merkkijono
            free(temp);       // Vapauta RiviNode-solmu
        }
    }
    free(ht); // Vapauta itse hajautustaulu
}


// Hae arvoa hajautustaulusta
void hae_arvoa_hajautustaulusta(RiviNode *node) {
    // Käy läpi linkitetty lista ja tulosta jokaisen solmun arvo
    while (node != NULL) {
        // Tulosta tämän solmun arvot
        for (int i = 0; i < MAX_SARAKKEET; i++) {
            printf("%s\n", node->rivi->arvot[i]);
        }
        printf("\n");

        // Siirry seuraavaan solmuun
        node = node->next;
    }
}

// void vapauta_sarakkeen_rivit(RiviNode *rivit) {
//     while (rivit != NULL) {
//         RiviNode *seuraava = rivit->seuraava;
//         free(rivit);  // Vapautetaan rivin solmu
//         rivit = seuraava;
//     }
// }

// // Tulostaa hajautustaulun sisällön
// void tulosta_hajautustaulu(HashTable *ht) {
//     for (int i = 0; i < MAX_SARAKKEET; i++) {
//         HashNode *solmu = ht->solmut[i];
//         printf("Sarake %d: %s\n", i + 1, solmu->nimi);
//         RiviNode *rivit = solmu->rivit;
//         while (rivit != NULL) {
//             printf("Arvot: ");
//             for (int j = 0; j < MAX_SARAKKEET; j++) {
//                 printf("%s: %s, ", solmu->nimi, rivit->rivi->arvot[j]);
//             }
//             printf("\n");
//             rivit = rivit->seuraava;
//         }
//     }
// }
