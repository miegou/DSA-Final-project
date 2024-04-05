#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "csv_reader.h"

#define MAX_SARAKE 30
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Käyttö: %s <tiedostonimi.csv>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *tiedostonimi = argv[1]; // Muutettu char* const char* -muotoon
    Sarake *sarakkeet = alusta_sarakkeet(tiedostonimi);
    Rivi **rivit = alusta_rivit(tiedostonimi);
    struct Node *root = NULL;

    int rivien_lkm = laske_rivien_lkm(tiedostonimi);

    int valinta;
    printf("Montako riviä haluat lukea tiedostosta? (enintään %d)\n", rivien_lkm);
    scanf("%d", &valinta);
    if (valinta > rivien_lkm || valinta < 1)
    {
        fprintf(stderr, "Virheellinen syöte.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < valinta; i++)
    {
        root = insertNode(root, rivit[i]->arvot[0]);
    }

    printf("Preorder-traversaali:\n");
    printPreOrder(root);
    printf("\n");

    // Vapautetaan varatut muistialueet
    for (int i = 0; i < valinta; i++)
    {
        for (int j = 0; j < MAX_SARAKE; j++)
        { // Korjattu MAX_SARAKE
            free(rivit[i]->arvot[j]);
        }
        free(rivit[i]);
    }
    free(rivit);

    freeAVL(root);
    free(sarakkeet);

    return 0;
}
