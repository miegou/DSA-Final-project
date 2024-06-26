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
        ht->hash_arvot[i] = 0;
    }
    printf("Hajautustaulu alustettu\n");
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
        uusi_ht->hash_arvot[i] = 0;
    }
    printf("Hajautustaulu luotu\n");
    return uusi_ht;
}

// Funktio hajautusarvon laskemiseen
unsigned int laske_hash(char *s) {
   unsigned hash = 0;

    for(; *s; ++s) // Käydään läpi merkkijono s, kunnes saavutetaan null-terminaattori ('\0')
    {
        hash += *s; // Lisätään merkin ASCII-arvo hash-muuttujaan
        hash += (hash << 10); // Sekoitusoperaatio: vasemmalle siirtäminen ja lisäys hash-arvoon
        hash ^= (hash >> 6); // Sekoitusoperaatio: bittien XOR-poisto hash-arvosta
    }

    hash += (hash << 3); // Sekoitusoperaatio: vasemmalle siirtäminen ja lisäys hash-arvoon
    hash ^= (hash >> 11); // Sekoitusoperaatio: bittien XOR-poisto hash-arvosta
    hash += (hash << 15); // Sekoitusoperaatio: vasemmalle siirtäminen ja lisäys hash-arvoon

    hash = hash % HASH_TAULUN_KOKO; // Hajautusarvon skaalaus taulukon koon mukaan

    return hash;
}


// Funktio rivien lisäämiseen hajautustauluun
void lisaa_rivit_hajautustauluun(HashTable **ht, Rivi **rivit, int rivien_maara, int rivin_indeksi_kategorialle) {
    for (int rivi_indeksi = 0; rivi_indeksi < rivien_maara; rivi_indeksi++) {
        char *nimi = rivit[rivi_indeksi]->arvot[rivin_indeksi_kategorialle];
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
            //printf("Uusi lista luotu hash-arvon kohdalle %d, ja solmu lisätty siihen.\n", hash);
        } else {
            // Jos hash-arvon kohdalla on jo lista
            int index = hash;
            int askelia = 0;
            while (askelia < HASH_TAULUN_KOKO) {
                RiviNode *current = (*ht)->hash_arvot[index];
                if (current == NULL) {
                    // Jos löydettiin vapaa indeksi, lisätään solmu siihen
                    (*ht)->hash_arvot[index] = new_node;
                    //printf("Solmu lisätty avoimen hajautuksen avulla indeksille %d.\n", index);
                    break;
                } else if (strcmp(current->nimi, nimi) == 0) {
                    // Jos solmun nimi on sama kuin uuden solmun nimi, lisää solmu listaan
                    while (current->next != NULL) {
                        current = current->next;
                    }
                    current->next = new_node;
                    //printf("Solmu lisätty olemassaolevaan listaan hash-arvon kohdalle %d.\n", hash);
                    break;
                } else {
                    // Jos solmun nimi ei ole sama, jatka seuraavaan indeksiin
                    index = (index + 1) % HASH_TAULUN_KOKO;
                    askelia++;
                }
            }
            if (askelia == HASH_TAULUN_KOKO) {
                printf("Taulukko on täynnä, lisäys epäonnistui.\n");
            }
        }
    }
    printf("Rivit lisätty hajautustauluun\n");
}


// Funktio hajautustaulle varatun muistin vapauttamiseen
void vapauta_hajautustaulu(HashTable *ht) {
    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        RiviNode *current = ht->hash_arvot[i];
        while (current != NULL) {
            RiviNode *temp = current;
            current = current->next;
            free(temp->nimi);
            free(temp);
        }
    }
    free(ht);
}

// Funktio hajautustaulun arvojen hakemiseen
void tulosta_arvot(HashTable *ht, ArvoJnr *erilaiset_arvot_jnroilla, int numeroindeksi, Sarake *sarakkeet) {
    // Etsi oikea rakenne tallennettujen arvojen joukosta, joka vastaa annettua järjestysnumeroa
    char *etsittava_arvo = NULL;
    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        if (erilaiset_arvot_jnroilla[i].jnr == numeroindeksi) {
            etsittava_arvo = erilaiset_arvot_jnroilla[i].arvo;
            break;
        }
    }

    // Jos annettu järjestysnumero ei vastaa tallennettua arvoa, tulosta virheilmoitus ja palaa
    if (etsittava_arvo == NULL) {
        printf("Arvoa järjestysnumerolla %d ei löytynyt rakenteesta.\n", numeroindeksi);
        return;
    }

    // Laske hash-arvo annetusta arvosta
    int hash = laske_hash(etsittava_arvo) % HASH_TAULUN_KOKO;

    // Tarkista, vastaako hajautustaulun indeksin ensimmäisen solmun arvo etsittyä arvoa
    while (ht->hash_arvot[hash] != NULL && strcmp(ht->hash_arvot[hash]->nimi, etsittava_arvo) != 0) {
        hash = (hash + 1) % HASH_TAULUN_KOKO;
    }

    // Jos hash-arvo ei vastaa etsittävää arvoa, tulosta virheilmoitus ja palaa
    if (ht->hash_arvot[hash] == NULL) {
        printf("Arvoa %s ei löytynyt hajautustaulusta.\n", etsittava_arvo);
        return;
    }

 // Tulosta löydetyt arvot
    printf("Löydettiin arvo: %s\n", etsittava_arvo);
    RiviNode *current = ht->hash_arvot[hash];
    while (current != NULL) {
        // Tulosta kaikki arvot linkitetystä listasta, jos ne eivät ole "ei arvoa"
        for (int j = 0; j < MAX_SARAKKEET; j++) {
            if (strcmp(current->rivi->arvot[j], "ei arvoa") != 0) {
                printf("%s: %s\n", sarakkeet[j].nimi, current->rivi->arvot[j]);
            }
        }
        printf("\n");
        current = current->next;
    }
}


// Funktio erilaisten arvojen laskemiseen
int laske_erilaiset_arvot(HashTable *ht, ArvoJnr *erilaiset_arvot_jnroilla, int numeroindeksi) {
    int erilaisten_maara = 0;

    // Etsi oikea rakenne tallennettujen arvojen joukosta, joka vastaa annettua järjestysnumeroa
    char *etsittava_arvo = NULL;
    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        if (erilaiset_arvot_jnroilla[i].jnr == numeroindeksi) {
            etsittava_arvo = erilaiset_arvot_jnroilla[i].arvo;
            break;
        }
    }

    // Jos annettu järjestysnumero ei vastaa tallennettua arvoa, palauta virhe
    if (etsittava_arvo == NULL) {
        printf("Arvoa järjestysnumerolla %d ei löytynyt rakenteesta.\n", numeroindeksi);
        return 0;
    }

    // Laske hash-arvo annetusta arvosta
    int hash = laske_hash(etsittava_arvo) % HASH_TAULUN_KOKO;

    // Tarkista, vastaako hajautustaulun indeksin ensimmäisen solmun arvo etsittyä arvoa
    while (ht->hash_arvot[hash] != NULL && strcmp(ht->hash_arvot[hash]->nimi, etsittava_arvo) != 0) {
        hash = (hash + 1) % HASH_TAULUN_KOKO;
    }

    // Jos hash-arvo ei vastaa etsittävää arvoa, palauta virhe
    if (ht->hash_arvot[hash] == NULL) {
        printf("Arvoa %s ei löytynyt hajautustaulusta.\n", etsittava_arvo);
        return 0;
    }

    // Löydettiin oikea arvo, käy läpi sen linkitetty lista ja laske erilaisten arvojen määrä
    printf("Löydettiin arvo: %s\n", etsittava_arvo);
    RiviNode *current = ht->hash_arvot[hash];
    while (current != NULL) {
        erilaisten_maara++;
        current = current->next;
    }

    return erilaisten_maara;
}


// Funktio hajautustaulun uudelleen jakamiseen
void jaa_hajautustaulu_uudelleen(HashTable **alkuperaiset_ht, ArvoJnr *arvot, int jarjestysluku, int sarakkeen_indeksi) {
    HashTable *alkuperaiset = *alkuperaiset_ht;
    HashTable *uusi_ht = luo_uusi_hajautustaulu();
    
    printf("Järjestysluku: %d\n", jarjestysluku);
    // Etsi annetulla järjestysluvulla vastaava arvo ArvoJnr-rakenteesta
    char *haluttu_arvo = NULL;
    int indeksi = 0; // Declare the "indeksi" variable
    if (jarjestysluku >= 0 && jarjestysluku < HASH_TAULUN_KOKO) {
        if (arvot[jarjestysluku].jnr == jarjestysluku) {
            haluttu_arvo = arvot[jarjestysluku].arvo;
            indeksi = arvot[jarjestysluku].tallennus_indeksi; // Assign a value to "indeksi"
        } else {
            printf("Virhe: Haluttua arvoa ei löytynyt annetulla järjestysluvulla. ArvoJnr-rakenteen jnr: %d, annettu järjestysluku: %d\n", arvot[jarjestysluku].jnr, jarjestysluku);
            return;
        }
    } else {
        printf("Virhe: Järjestysluku ei ole sallitulla alueella.\n");
        return;
    }

    // Tulostetaan löydetty haluttu arvo ja järjestysluku
    if (haluttu_arvo != NULL) {
        printf("Löydetty haluttu arvo: %s, järjestysluku: %d\n", haluttu_arvo, jarjestysluku);
    } else {
        printf("Virhe: Haluttua arvoa ei löydetty annetulla järjestysluvulla.\n");
        return;
    }

    // Käy läpi vain annetun indeksin solmut alkuperäisessä hajautustaulussa
    RiviNode *current = alkuperaiset->hash_arvot[indeksi];
    while (current != NULL) {
        if (current->rivi->arvot[sarakkeen_indeksi] != NULL) {
            char *uuden_indeksin_arvo = current->rivi->arvot[sarakkeen_indeksi];
            // Lasketaan hash uuden indeksin arvon perusteella
            int hash = laske_hash(uuden_indeksin_arvo) % HASH_TAULUN_KOKO;
            
            // Luodaan uusi solmu
            RiviNode *new_node = malloc(sizeof(RiviNode));
            if (new_node == NULL) {
                perror("Muistin varaus epäonnistui");
                exit(EXIT_FAILURE);
            }
            new_node->rivi = current->rivi;
            new_node->next = NULL;
            new_node->nimi = strdup(uuden_indeksin_arvo);
            
            // Tarkista, onko hash-arvon kohdalla jo lista
            if (uusi_ht->hash_arvot[hash] == NULL) {
                // Jos hash-arvon kohdalla ei ole vielä listaa, lisää uusi solmu sinne
                uusi_ht->hash_arvot[hash] = new_node;
            } else {
                // Jos hash-arvon kohdalla on jo lista, käytä avointa hajautusta
                int index = hash;
                int askelia = 0;
                while (askelia < HASH_TAULUN_KOKO) {
                    RiviNode *existing_node = uusi_ht->hash_arvot[index];
                    if (existing_node == NULL) {
                        // Jos löydettiin vapaa indeksi, lisätään solmu siihen
                        uusi_ht->hash_arvot[index] = new_node;
                        break;
                    } else if (strcmp(existing_node->nimi, uuden_indeksin_arvo) == 0) {
                        // Jos solmun nimi on sama kuin uuden solmun nimi, lisää solmu listaan
                        while (existing_node->next != NULL) {
                            existing_node = existing_node->next;
                        }
                        existing_node->next = new_node;
                        break;
                    } else {
                        // Jos solmun nimi ei ole sama, jatka seuraavaan indeksiin
                        index = (index + 1) % HASH_TAULUN_KOKO;
                        askelia++;
                    }
                }
                // Tarkista, jos taulukko on täynnä
                if (askelia == HASH_TAULUN_KOKO) {
                    printf("Taulukko on täynnä, lisäys epäonnistui.\n");
                    free(new_node->nimi);
                    free(new_node);
                }
            }
        }
        current = current->next;
    }

    // Vaihda alkuperäisen hajautustaulun osoitin uuteen
    *alkuperaiset_ht = uusi_ht;
    printf("Hajautustaulu jaettu uudelleen\n");
}


// Funktio erilaisten arvojen luomiseen ArvoJnr-rakenteesta
ArvoJnr *luo_arvojnr_listasta(HashTable *ht, int rivin_indeksi_kategorialle) {
    ArvoJnr *erilaiset_arvot_jnroilla = malloc(sizeof(ArvoJnr) * HASH_TAULUN_KOKO);
    if (erilaiset_arvot_jnroilla == NULL) {
        perror("Muistin varaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }

    // Alusta erilaiset_arvot_jnroilla-rakenne
    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        erilaiset_arvot_jnroilla[i].arvo = NULL;
        erilaiset_arvot_jnroilla[i].jnr = 0;
        erilaiset_arvot_jnroilla[i].tallennus_indeksi = 0; // Alustetaan tallennus_indeksi -1:ksi
    }

    int erilaisten_maara = 0;
    int jnr = 0;
    int *indeksit = malloc(sizeof(int) * HASH_TAULUN_KOKO); // Tallentaa käsitellyt indeksit

    // Alusta indeksit
    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        indeksit[i] = 0;
    }

    // Käy läpi kaikkien hajautustaulun indeksien ensimmäiset solmut
    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        if (!indeksit[i]) { // Jos indeksiä ei ole vielä käsitelty
            RiviNode *current = ht->hash_arvot[i];
            if (current != NULL) {
                char *arvo = current->rivi->arvot[rivin_indeksi_kategorialle];

                // Lisää arvo ja tallennusindeksi suoraan erilaiset_arvot_jnroilla-rakenteeseen
                erilaiset_arvot_jnroilla[erilaisten_maara].arvo = strdup(arvo);
                erilaiset_arvot_jnroilla[erilaisten_maara].jnr = jnr;
                erilaiset_arvot_jnroilla[erilaisten_maara].tallennus_indeksi = i; // Tallenna indeksi
                (erilaisten_maara)++;
                jnr++;

                // Merkitse indeksi käsitellyksi
                indeksit[i] = 1;
            }
        }
    }
    free(indeksit); // Vapauta indeksitaulu
    return erilaiset_arvot_jnroilla;
}

// Tulostaa ArvoJnr-rakenteen arvot
void tulosta_arvojnr_lista(ArvoJnr *arvot) {
    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        if (arvot[i].arvo != NULL && arvot[i].jnr != -1) {
            printf("%d: %s\n", arvot[i].jnr, arvot[i].arvo);
        }
    }
}
