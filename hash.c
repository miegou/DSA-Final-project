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

// Funktio hajautustaulun luomiseen
HashTable *luo_uusi_hajautustaulu() {
    HashTable *uusi_ht = malloc(sizeof(HashTable));
    if (uusi_ht == NULL) {
        perror("Muistin varaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Alusta hajautustaulun arvot
    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        uusi_ht->hash_arvot[i] = 0; // Voit alustaa arvot haluamallasi tavalla
    }
    printf("Hajautustaulu luotu\n");
    return uusi_ht;
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


void lisaa_rivit_hajautustauluun(HashTable **ht, Rivi **rivit, int rivien_maara, int sarakkeen_indeksi) {
    for (int rivi_indeksi = 0; rivi_indeksi < rivien_maara; rivi_indeksi++) {
        char *nimi = rivit[rivi_indeksi]->arvot[sarakkeen_indeksi];
        int hash = laske_hash(nimi) % HASH_TAULUN_KOKO; // Laske hash-arvo ja moduloi taulukon koon kanssa
        
        // Luodaan uusi RiviNode
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
                    index = 0;
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

void hae_arvoa_hajautustaulusta(HashTable *ht, ArvoJnr *arvot) {
    int valittu_jnr;
    printf("Syötä haluamasi arvon järjestysnumero: ");
    scanf("%d", &valittu_jnr);

    char *etsittava_arvo = arvot[valittu_jnr - 1].arvo; // Vähennä yksi, koska indeksit alkavat nollasta
    int hash = laske_hash(etsittava_arvo) % HASH_TAULUN_KOKO;

    // Aloita haku oletetusta indeksistä
    // Tarkista ensin, että oletettu indeksi on validi
    if (ht->hash_arvot[hash] != NULL && strcmp(ht->hash_arvot[hash]->nimi, etsittava_arvo) == 0) {
        // Tulosta kaikki solmut linkitetystä listasta
        printf("Löytyi arvo: %s\n", etsittava_arvo);
        RiviNode *current = ht->hash_arvot[hash];
        while (current != NULL) {
            for (int j = 0; j < MAX_SARAKKEET; j++) {
                printf("%s\n", current->rivi->arvot[j]);
            }
            printf("\n");
            current = current->next;
        }
        return; // Lopeta funktio, kun arvo on löydetty ja tulostettu
    }

    // Jos oletettu indeksi ei sisällä etsittävää arvoa, jatka haku seuraavista indekseistä
    for (int i = (hash + 1) % HASH_TAULUN_KOKO; i != hash; i = (i + 1) % HASH_TAULUN_KOKO) {
        // Tarkista, että indeksissä on solmu ja sen arvo täsmää etsittävän arvon kanssa
        if (ht->hash_arvot[i] != NULL && strcmp(ht->hash_arvot[i]->nimi, etsittava_arvo) == 0) {
            // Tulosta kaikki solmut linkitetystä listasta
            printf("Löytyi arvo: %s\n", etsittava_arvo);
            RiviNode *current = ht->hash_arvot[i];
            while (current != NULL) {
                for (int j = 0; j < MAX_SARAKKEET; j++) {
                    printf("%s\n", current->rivi->arvot[j]);
                }
                printf("\n");
                current = current->next;
            }
            return; // Lopeta funktio, kun arvo on löydetty ja tulostettu
        }
    }

    // Jos arvoa ei löytynyt hajautustaulusta, tulosta ilmoitus
    printf("Arvoa %s ei löytynyt hajautustaulusta.\n", etsittava_arvo);
}





// Laske erilaiset arvot hajautustaulusta
int laske_erilaiset_arvot(HashTable *ht, int hajautettavan_arvon_indeksi) {
    int erilaisten_maara = 0;
    RiviNode *current = ht->hash_arvot[hajautettavan_arvon_indeksi];
    while (current != NULL) {
        erilaisten_maara++;
        current = current->next;
    }
    return erilaisten_maara;
}


ArvoJnr *nayta_erilaiset_arvot_karsituista(HashTable *ht, int indeksi, int sarakkeen_indeksi) {
    ArvoJnr *erilaiset_arvot_jnroilla = malloc(sizeof(ArvoJnr) * HASH_TAULUN_KOKO);
    if (erilaiset_arvot_jnroilla == NULL) {
        perror("Muistin varaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Alusta löydetyt arvot
    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        erilaiset_arvot_jnroilla[i].arvo = NULL;
        erilaiset_arvot_jnroilla[i].jnr = -1;
    }

    // Käy läpi halutun indeksin listat hajautustaulussa
    RiviNode *current = ht->hash_arvot[indeksi];
    int erilaisten_maara = 0;
    int jnr = 1;
    while (current != NULL) {
        char *arvo = current->rivi->arvot[sarakkeen_indeksi];
        // Tarkista, onko arvo jo tallennettu
        int loydetty = 0;
        for (int i = 0; i < erilaisten_maara; i++) {
            if (strcmp(arvo, erilaiset_arvot_jnroilla[i].arvo) == 0) {
                loydetty = 1;
                break;
            }
        }
        // Jos arvoa ei ole vielä tallennettu, lisätään se listaan
        if (!loydetty) {
            erilaiset_arvot_jnroilla[erilaisten_maara].arvo = strdup(arvo);
            erilaiset_arvot_jnroilla[erilaisten_maara].jnr = jnr;
            printf("%d. %s %d\n", jnr, arvo, laske_hash(arvo));
            (erilaisten_maara)++;
            jnr++;
        }
        current = current->next;
    }

    printf("Löytyi %d erilaista arvoa.\n", erilaisten_maara);
    return erilaiset_arvot_jnroilla;
}



void jaa_hajautustaulu_uudelleen(HashTable **alkuperaiset_ht, int alkuperaisen_indeksi, int uuden_indeksi) {
    HashTable *alkuperaiset = *alkuperaiset_ht;
    HashTable *uusi_ht = luo_uusi_hajautustaulu();
    RiviNode *current = alkuperaiset->hash_arvot[alkuperaisen_indeksi];
    while (current != NULL) {
        if (current->rivi->arvot[uuden_indeksi] != NULL) {
            char *uuden_indeksin_arvo = current->rivi->arvot[uuden_indeksi];
            //printf("Uuden indeksin arvo: %s\n", uuden_indeksin_arvo);
            // Lasketaan hash uuden indeksin arvon perusteella
            int hash = laske_hash(uuden_indeksin_arvo) % HASH_TAULUN_KOKO;
            // Jos hash-arvon kohdalla on jo lista
            if (uusi_ht->hash_arvot[hash] == NULL) {
                // Jos hash-arvon kohdalla ei ole vielä listaa, luodaan uusi lista ja lisätään sinne uusi solmu
                RiviNode *new_node = malloc(sizeof(RiviNode));
                if (new_node == NULL) {
                    perror("Muistin varaus epäonnistui");
                    exit(EXIT_FAILURE);
                }
                new_node->rivi = current->rivi;
                new_node->next = NULL;
                new_node->nimi = strdup(uuden_indeksin_arvo);
                uusi_ht->hash_arvot[hash] = new_node;
                //printf("Luotiin uusi lista arvolle: %s, indeksillä: %d\n", new_node->nimi, hash); 
            } else {
                // Jos hash-arvon kohdalla on jo lista, tarkista ensimmäisen solmun nimi
                RiviNode *existing_node = uusi_ht->hash_arvot[hash];
                if (strcmp(existing_node->nimi, uuden_indeksin_arvo) == 0) {
                    // Jos ensimmäisen solmun nimi on sama kuin uuden solmun nimi, lisää solmu listaan
                    while (existing_node->next != NULL) {
                        existing_node = existing_node->next;
                    }
                    RiviNode *new_node = malloc(sizeof(RiviNode));
                    if (new_node == NULL) {
                        perror("Muistin varaus epäonnistui");
                        exit(EXIT_FAILURE);
                    }
                    new_node->rivi = current->rivi;
                    new_node->next = NULL;
                    new_node->nimi = strdup(uuden_indeksin_arvo);
                    existing_node->next = new_node;
                    //printf("Listaan lisättiin uusi solmu arvolla: %s, indeksillä: %d\n", new_node->nimi, hash);
                } else {
                    // Jos ensimmäisen solmun nimi ei ole sama kuin uuden solmun nimi, käytä avointa hajautusta
                    int index = (hash + 1) % HASH_TAULUN_KOKO;
                    while (uusi_ht->hash_arvot[index] != NULL && index != hash) {
                        // Tarkista jokaiselta avoimeen hajautukseen siirtyvältä indeksiltä, onko siellä saman nimisiä solmuja
                        if (strcmp(uusi_ht->hash_arvot[index]->nimi, uuden_indeksin_arvo) == 0) {
                            // Jos saman niminen solmu löytyy, lisää uusi solmu listaan ja lopeta
                            RiviNode *new_node = malloc(sizeof(RiviNode));
                            if (new_node == NULL) {
                                perror("Muistin varaus epäonnistui");
                                exit(EXIT_FAILURE);
                            }
                            new_node->rivi = current->rivi;
                            new_node->next = uusi_ht->hash_arvot[index]->next;
                            new_node->nimi = strdup(uuden_indeksin_arvo);
                            uusi_ht->hash_arvot[index]->next = new_node;
                            //printf("Listaan lisättiin uusi solmu arvolla: %s, indeksillä: %d\n", new_node->nimi, index);
                            break;
                        }
                        index = (index + 1) % HASH_TAULUN_KOKO;
                    }
                    // Jos löydettiin vapaa indeksi, lisätään solmu siihen
                    if (uusi_ht->hash_arvot[index] == NULL) {
                        RiviNode *new_node = malloc(sizeof(RiviNode));
                        if (new_node == NULL) {
                            perror("Muistin varaus epäonnistui");
                            exit(EXIT_FAILURE);
                        }
                        new_node->rivi = current->rivi;
                        new_node->next = NULL;
                        new_node->nimi = strdup(uuden_indeksin_arvo);
                        uusi_ht->hash_arvot[index] = new_node;
                        //printf("Listaan lisättiin uusi solmu arvolla: %s, lisättiin indeksiin: %d ettei tule törmäyksiä.\n", new_node->nimi, index);
                    } else {
                        index = 0;
                    }
                }
            }
        }
        current = current->next;
    }
    *alkuperaiset_ht = uusi_ht;
    printf("Hajautustaulu jaettu uudelleen\n");
}

