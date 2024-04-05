#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "csv_reader.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Käyttö: %s <tiedostonimi.csv>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *tiedostonimi = argv[1];
    Sarake *sarakkeet = alusta_sarakkeet(tiedostonimi);
    Rivi **rivit = alusta_rivit(tiedostonimi);
    struct Node *root = NULL;

    int rivien_lkm = laske_rivien_lkm(tiedostonimi);

    int valinta;
    printf("Montako riviä haluat lukea tiedostosta? (enintään %d)\n", rivien_lkm);
    scanf("%d", &valinta);
    if (valinta > rivien_lkm || valinta < 1) {
        fprintf(stderr, "Virheellinen syöte.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < valinta; i++) {
        root = insertNode(root, rivit[i]->arvot[0]);
    }

    // Print the content of the first node in the AVL tree
    printf("Yhden solmun sisältö:\n");
    printNode(root);

    //     // Kysytään käyttäjältä millä arvolla halutaan vertailla puuta
    // char vertailtava[MAX_RIVI_PITUUS];
    // printf("Anna arvo, jolla haluat vertailla puuta: ");
    // scanf("%s", vertailtava);

    // // Tulostetaan listan vertailuarvon mukainen lista
    // printf("Vertailuarvon mukainen lista:\n");
    // tulosta_vertailuarvolla(root, vertailtava);

    // Tulosta ensimmäisen solmun otsikko ja arvo
    if (root != NULL) {
        printf("Ensimmäisen solmun otsikko: %s\n", sarakkeet[0].nimi);
        printf("Ensimmäisen solmun arvo: %s\n", root->nimi);
    } else {
        printf("Puussa ei ole solmuja\n");
    }

    // Free memory of AVL tree
    freeAVL(root);

    return 0;
}
