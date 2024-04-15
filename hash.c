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

    // Alusta hajautustaulun solmut
    for (int i = 0; i < MAX_SARAKKEET; i++) {
        ht->solmut[i] = NULL;
    }
    printf("Hajautustaulu luotu\n");
    return ht;
}


// Yksinkertainen hajautusfunktio
int hajautusfunktio(const char *avain) {
    int summa = 0;
    for (int i = 0; avain[i] != '\0'; i++) {
        summa += avain[i];
    }
    return summa % MAX_SARAKKEET;
}


void lisaa_sarakkeet_hajautustauluun(HashTable *ht, Sarake *sarakkeet) {
    for (int i = 0; i < MAX_SARAKKEET; i++) {
        ht->solmut[i] = malloc(sizeof(HashNode));
        if (ht->solmut[i] == NULL) {
            perror("Muistin varaaminen epäonnistui");
            exit(EXIT_FAILURE);
        }
        printf("%d. %s\n", i + 1, sarakkeet[i].nimi);
        strcpy(ht->solmut[i]->nimi, sarakkeet[i].nimi);
        ht->solmut[i]->rivit = NULL; // Alustetaan linkitetty lista nulleksi
    }
}


void lisaa_rivi(HashTable *ht, Sarake *sarakkeet, char (*rivin_arvot)[MAX_ARVO_PITUUS]) {
    // Käydään läpi kaikki rivin arvot ja lisätään ne hajautustauluun
    for (int i = 0; i < MAX_SARAKKEET; i++) {
        char *sarakkeen_nimi = sarakkeet[i].nimi; // Käytetään sarakkeiden nimiä taulukosta
        char *arvo = rivin_arvot[i]; // Käytetään rivin arvoja taulukosta
        lisaa_sarake_arvo(ht, sarakkeen_nimi, arvo);
    }
}


void lisaa_sarake_arvo(HashTable *ht, char *sarakkeen_nimi, char *arvo) {
    // Etsi hajautustaulusta saraketta vastaava solmu
    int indeksi = hajautusfunktio(sarakkeen_nimi);
    HashNode *solmu = ht->solmut[indeksi];

    // Jos solmua ei ole vielä luotu, luo uusi solmu ja liitä se hajautustauluun
    if (solmu == NULL) {
        solmu = malloc(sizeof(HashNode));
        if (solmu == NULL) {
            perror("Muistin varaaminen epäonnistui");
            exit(EXIT_FAILURE);
        }
        strcpy(solmu->nimi, sarakkeen_nimi);
        solmu->rivit = NULL;
        ht->solmut[indeksi] = solmu;
    }

    // Lisää uusi rivi (tai solmu) sarakkeen linkitettyyn listaan
    RiviNode *uusi_rivi = malloc(sizeof(RiviNode));
    if (uusi_rivi == NULL) {
        perror("Muistin varaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }
    uusi_rivi->rivi = malloc(sizeof(Rivi));
    if (uusi_rivi->rivi == NULL) {
        perror("Muistin varaaminen epäonnistui");
        exit(EXIT_FAILURE);
    }
    strcpy(uusi_rivi->rivi->arvot[indeksi], arvo);
    uusi_rivi->seuraava = solmu->rivit;
    solmu->rivit = uusi_rivi;
}

void vapauta_hajautustaulu(HashTable *ht) {
    for (int i = 0; i < MAX_SARAKKEET; i++) {
        HashNode *current = ht->solmut[i];
        while (current != NULL) {
            HashNode *next = current->seuraava;
            vapauta_sarakkeen_rivit(current->rivit); // Vapauta linkitetyn listan muisti
            free(current); // Vapauta solmun muisti
            current = next;
        }
    }
    free(ht); // Vapauta hajautustaulun muisti
}

void vapauta_sarakkeen_rivit(RiviNode *rivit) {
    while (rivit != NULL) {
        RiviNode *seuraava = rivit->seuraava;
        free(rivit);  // Vapautetaan rivin solmu
        rivit = seuraava;
    }
}

// Tulostaa hajautustaulun sisällön
void tulosta_hajautustaulu(HashTable *ht) {
    for (int i = 0; i < MAX_SARAKKEET; i++) {
        HashNode *solmu = ht->solmut[i];
        printf("Sarake %d: %s\n", i + 1, solmu->nimi);
        RiviNode *rivit = solmu->rivit;
        while (rivit != NULL) {
            printf("Arvot: ");
            for (int j = 0; j < MAX_SARAKKEET; j++) {
                printf("%s: %s, ", solmu->nimi, rivit->rivi->arvot[j]);
            }
            printf("\n");
            rivit = rivit->seuraava;
        }
    }
}


// Etsii tuotteet hajautustaulusta sarakkeen avaimen perusteella ja tulostaa niihin liittyvät tiedot
void etsi_tuotteet_sarakkeen_perusteella(HashTable *ht, char *sarakkeen_nimi, char *etsittava_arvo) {
    // Käydään läpi hajautustaulun solmut
    for (int i = 0; i < ht->koko; i++) {
        HashNode *current = ht->solmut[i];
        printf("Solmu %d: %s\n", i + 1, current->nimi);

        // Tarkistetaan, että solmun rivitietoja on saatavilla
        if (current->rivit != NULL && current->rivit->rivi != NULL) {
            // Tulostetaan vertailtavat arvot
            printf("Vertaillaan saraketta %s: vertailuarvo %s, etsittävä arvo %s\n", sarakkeen_nimi, current->rivit->rivi->arvot, etsittava_arvo);

            // Tarkistetaan, vastaako solmun sarakkeen arvo etsittävää arvoa
            if (strcmp(current->nimi, sarakkeen_nimi) == 0 && strcmp(current->rivit->rivi->arvot[2], etsittava_arvo) == 0) {
                // Tulostetaan solmun kaikki tiedot
                printf("Tuote löytyi:\n");
                printf("Nimi: %s\n", current->nimi);
                // Lisää tässä tarvittaessa muut tiedot
            }
        } else {
            printf("Rivitietoja ei löytynyt\n");
        }
    }
}


