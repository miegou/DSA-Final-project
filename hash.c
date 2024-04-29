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


void lisaa_rivit_hajautustauluun(HashTable **ht, Rivi **rivit, int rivien_maara, int rivin_indeksi_kategorialle) {
    for (int rivi_indeksi = 0; rivi_indeksi < rivien_maara; rivi_indeksi++) {
        char *nimi = rivit[rivi_indeksi]->arvot[rivin_indeksi_kategorialle];
        int hash = laske_hash(nimi) % HASH_TAULUN_KOKO; // Laske hash-arvo ja moduloi taulukon koon kanssa
        
        // Luodaan uusi RiviNode
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

        // Tulosta lisättävän rivin arvo ja siihen verrattava arvo
        //printf("Lisättävä arvo: %s, verrataan hash-arvon kohdalla olevaan arvoon.\n", nimi);


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
    // Tarkista ensin, että oletettu rivin_indeksi_kategorialle on validi
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

    // Jos oletettu rivin_indeksi_kategorialle ei sisällä etsittävää arvoa, jatka haku seuraavista indekseistä
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


ArvoJnr *nayta_erilaiset_arvot_karsituista(HashTable *ht, int rivin_indeksi_kategorialle) {
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
                // Tulosta testituloste
                //printf("Indeksi: %d, Ensimmäisen solmun nimi: %s\n", i, current->nimi);

                // Lisää arvo suoraan erilaisten arvojen listaan
                erilaiset_arvot_jnroilla[erilaisten_maara].arvo = strdup(arvo);
                erilaiset_arvot_jnroilla[erilaisten_maara].jnr = jnr;
                printf("%d. %s %d\n", jnr, arvo, laske_hash(arvo));
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



void jaa_hajautustaulu_uudelleen(HashTable **alkuperaiset_ht, ArvoJnr *arvot, int jarjestysluku) {
    HashTable *alkuperaiset = *alkuperaiset_ht;
    HashTable *uusi_ht = luo_uusi_hajautustaulu();
    
    printf("Järjestysluku: %d\n", jarjestysluku);
    // Etsi annetulla järjestysluvulla vastaava arvo ArvoJnr-rakenteesta
    char *haluttu_arvo = NULL;
    if (jarjestysluku >= 0 && jarjestysluku < HASH_TAULUN_KOKO) {
        if (arvot[jarjestysluku].jnr == jarjestysluku) {
            haluttu_arvo = arvot[jarjestysluku].arvo;
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

// Tulosta alkuperäisen hajautustaulun solmut ja niiden arvot
printf("Alkuperäisen hajautustaulun solmut:\n");
for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
    RiviNode *current = alkuperaiset->hash_arvot[i];
    while (current != NULL) {
        if (current->rivi != NULL && current->rivi->arvot != NULL) {
            if (current->rivi->arvot[jarjestysluku] != NULL) {
                printf("Solmu löydetty: %s\n", current->rivi->arvot[jarjestysluku]);
            } else {
                printf("Solmu löydetty, mutta sen arvo on NULL.\n");
            }
        } else {
            printf("Virhe: Solmu tai sen arvot eivät ole alustettuja.\n");
        }
        current = current->next;
    }
}


    // Käy läpi alkuperäisen hajautustaulun solmut
    for (int i = 0; i < HASH_TAULUN_KOKO; i++) {
        RiviNode *current = alkuperaiset->hash_arvot[i];
        while (current != NULL) {
            if (current->rivi->arvot[jarjestysluku] != NULL) {
                char *uuden_indeksin_arvo = current->rivi->arvot[jarjestysluku];
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
    }
    
    // Vaihda alkuperäisen hajautustaulun osoitin uuteen
    *alkuperaiset_ht = uusi_ht;
    printf("Hajautustaulu jaettu uudelleen\n");
}




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

