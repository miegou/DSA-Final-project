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
    Rivi **rivit = alusta_rivit(tiedostonimi, sarakkeet);
    struct Node *root = NULL;

    int sarakkeiden_lkm = 0;
    while (sarakkeet[sarakkeiden_lkm].nimi[0] != '\0') {
        sarakkeiden_lkm++;
    }

    int rivien_lkm = laske_rivien_lkm(tiedostonimi);

    int valinta;
    printf("Montako riviä haluat lukea tiedostosta? (enintään %d)\n", rivien_lkm);
    scanf("%d", &valinta);
    if (valinta > rivien_lkm || valinta < 1) {
        fprintf(stderr, "Virheellinen syöte.\n");
        exit(EXIT_FAILURE);
    }

    // Tulosta sarakkeiden nimet käyttäjälle
    printf("Valitse vertailuarvo sarakkeen perusteella:\n");
    for (int i = 0; i < sarakkeiden_lkm; i++) {
        printf("%d. %s\n", i + 1, sarakkeet[i].nimi);
    }

    // Pyydä käyttäjää valitsemaan sarakkeen numero
    printf("Anna sarakkeen numero, jolla haluat vertailla puuta: ");
    scanf("%d", &valinta);

    // Tarkista, että käyttäjän antama valinta on kelvollinen
    if (valinta < 1 || valinta > sarakkeiden_lkm) {
        fprintf(stderr, "Virheellinen syöte.\n");
        exit(EXIT_FAILURE);
    }

    // Valitse vertailuarvo valitun sarakkeen perusteella
    char vertailuarvo[MAX_RIVI_PITUUS];
    strcpy(vertailuarvo, sarakkeet[valinta - 1].nimi);

    // Lisää solmut puuhun valitun vertailuarvon perusteella
    for (int i = 0; i < valinta; i++) {
        root = insertNode(root, rivit[i], sarakkeet, sarakkeiden_lkm, vertailuarvo);
    }

    // Print the content of the first node in the AVL tree
    printf("Yhden solmun sisältö:\n");
    printNode(root);

    // Free memory of AVL tree
    freeAVL(root);

    return 0;
}

