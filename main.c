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

    const char *tiedostonimi = argv[1];
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

    // Function to print AVL tree in preorder traversal
    void printPreOrder(struct Node *root) {
        if (root != NULL) {
            printf("%s ", root->nimi);
            printPreOrder(root->left);
            printPreOrder(root->right);
        }
    }

    // Funktio vapauttaa AVL-puun muistin rekursiivisesti
    void vapauta_puu(struct Node *node) {
        if (node == NULL) return;

        // Vapautetaan vasen ja oikea alipuu
        vapauta_puu(node->left);
        vapauta_puu(node->right);

        // Vapautetaan solmu itse
        free(node);
    }

    return 0; // Lisätty return-lause
}
