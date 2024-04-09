#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "csv_reader.h"

#define TIEDOSTONIMI "alkon-hinnasto-tekstitiedostona.csv"

int main() {
    const char *tiedostonimi = TIEDOSTONIMI;
    Sarake *sarakkeet = alusta_sarakkeet(tiedostonimi);
    Rivi **rivit = alusta_rivit(tiedostonimi, sarakkeet);
    struct Node *root = NULL;

    int sarakkeiden_lkm = MAX_SARAKKEET;

    printf("Sarakkeiden lukumäärä: %d\n", sarakkeiden_lkm);

    printf("Sarakkeiden nimet:\n");
    for (int i = 0; i < sarakkeiden_lkm; i++) {
        printf("%d. %s\n", i + 1, sarakkeet[i].nimi);
    }

    int rivien_lkm = laske_rivien_lkm(tiedostonimi);

    int rivien_maara = 0;
    printf("Montako riviä haluat lukea tiedostosta? (enintään %d)\n", rivien_lkm);
    scanf("%d", &rivien_maara);
    if (rivien_maara > rivien_lkm || rivien_maara < 1) {
        fprintf(stderr, "Virheellinen syöte.\n");
        exit(EXIT_FAILURE);
    }

    printf("Valitse vertailuarvo sarakkeen perusteella:\n");
    for (int i = 0; i < sarakkeiden_lkm; i++) {
        printf("%d. %s\n", i + 1, sarakkeet[i].nimi);
    }

    int sarakkeen_numero = 0;
    printf("Anna sarakkeen numero, jolla haluat vertailla puuta: ");
    scanf("%d", &sarakkeen_numero);

    if (sarakkeen_numero < 1 || sarakkeen_numero > sarakkeiden_lkm) {
        fprintf(stderr, "Virheellinen syöte.\n");
        exit(EXIT_FAILURE);
    }

    char vertailuarvo[MAX_RIVI_PITUUS];
    strcpy(vertailuarvo, sarakkeet[sarakkeen_numero - 1].nimi);
    printf("Valittu sarakkeen nimi: %s\n", vertailuarvo);

    for (int i = 0; i < rivien_maara; i++) {
        printf("Iteraatio %d\n", i + 1);
        root = insertNode(root, rivit[i], sarakkeet, sarakkeiden_lkm, vertailuarvo);
        if (root == NULL) {
            printf("Juurisolmun päivitys epäonnistui iteraatiossa %d\n", i + 1);
        } else {
            printf("Juurisolmun päivitys onnistui iteraatiossa %d\n", i + 1);
        }
    }
    printNode(root, sarakkeet);

    freeAVL(root);

    return 0;
}
