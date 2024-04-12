#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "csv_reader.h"

struct Node *newNode(Rivi *rivi, Sarake *sarakkeet, int sarakkeiden_lkm) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the node's data
    for (int i = 0; i < sarakkeiden_lkm; ++i) {
        strcpy(node->arvot[i], rivi->arvot[i]);
    }
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// Function to perform right rotation
struct Node *rightRotate(struct Node *y) {
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Function to perform left rotation
struct Node *leftRotate(struct Node *x) {
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Laskee solmun korkeuden
int height(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Palauttaa suuremman arvon kahdesta annetusta
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Laskee tasapainon solmun kohdalla
int getBalance(struct Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}


struct Node *insertNode(struct Node *node, Rivi *rivi, Sarake *sarakkeet, int sarakkeiden_lkm, char *vertailuarvo) {
    // Find the correct position to insert the node and insert it
    if (node == NULL)
        return (newNode(rivi, sarakkeet, sarakkeiden_lkm));

    // Find the index of the column based on the comparison value
    int sarakkeen_indeksi = -1;
    for (int i = 0; i < sarakkeiden_lkm; ++i) {
        if (strcmp(sarakkeet[i].nimi, vertailuarvo) == 0) {
            sarakkeen_indeksi = i;
            break;
        }
    }

    if (sarakkeen_indeksi == -1) {
        fprintf(stderr, "Error: Column not found.\n");
        return NULL;
    }

    int compare = strcmp(rivi->arvot[sarakkeen_indeksi], node->arvot[sarakkeen_indeksi]);
    if (compare < 0)
        node->left = insertNode(node->left, rivi, sarakkeet, sarakkeiden_lkm, vertailuarvo);
    else if (compare > 0)
        node->right = insertNode(node->right, rivi, sarakkeet, sarakkeiden_lkm, vertailuarvo);
    else
        return node;



    node->height = 1 + max(height(node->left),
                height(node->right));

    int balance = getBalance(node);
    if (balance > 1 && strcmp(rivi->arvot[sarakkeen_indeksi], node->left->arvot[sarakkeen_indeksi]) < 0)
        return rightRotate(node);

    if (balance < -1 && strcmp(rivi->arvot[sarakkeen_indeksi], node->right->arvot[sarakkeen_indeksi]) > 0)
        return leftRotate(node);

    if (balance > 1 && strcmp(rivi->arvot[sarakkeen_indeksi], node->left->arvot[sarakkeen_indeksi]) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(rivi->arvot[sarakkeen_indeksi], node->right->arvot[sarakkeen_indeksi]) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}



// Function to find the node with minimum value in a tree
struct Node *minValueNode(struct Node *node) {
    struct Node *current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}



// Function to print AVL tree in preorder traversal
void printPreOrder(struct Node *root) {
    if (root != NULL) {
        printf("Node Content:\n");
        printf("Sarakkeet:\n");
        for (int i = 0; i < MAX_SARAKKEET; i++) {
            printf("%s: %s\n", root->arvot[i], root->arvot[i]);
        }
        printPreOrder(root->left);
        printPreOrder(root->right);
    }
}

// Function to print AVL tree in inorder traversal (alphabetical order)
void printInOrder(struct Node *root) {
    if (root != NULL) {
        printInOrder(root->left);
        printf("Node Content:\n");
        printf("Sarakkeet:\n");
        for (int i = 0; i < MAX_SARAKKEET; i++) {
            printf("%s: %s\n", root->arvot[i], root->arvot[i]);
        }
        printInOrder(root->right);
    }
}



void freeAVL(struct Node *node) {
    if (node == NULL) return;

    // Free left and right subtrees
    freeAVL(node->left);
    freeAVL(node->right);

    // Free the node itself
    free(node);
}

// Funktio yhden AVL-puun solmun sisällön tulostamiseksi
void printNode(struct Node *node, Sarake *sarakkeet) {
    if (node != NULL) {
        printf("Solmun sisältö:\n");

        // Tulostetaan jokaisen sarakkeen nimi ja arvo omalle riville
        for (int i = 0; i < MAX_SARAKKEET; ++i) {
            printf("%s: %s\n", sarakkeet[i].nimi, node->arvot[i]);
        }

        // Tulostetaan vasen ja oikea lapsisolmu sekä korkeus
        printf("Vasen lapsi: %p\n", (void *)node->left);
        printf("Oikea lapsi: %p\n", (void *)node->right);
        printf("Korkeus: %d\n", node->height);
    } else {
        printf("Solmu on tyhjä\n");
    }
}

void printNodesWithColumnValue(struct Node* node, int column, char* targetValue) {
    if (node == NULL) {
        return;
    }

    // Tarkista, onko haetun sarakkeen arvo haluttu
    if (strcmp(node->arvot[column], targetValue) == 0) {
        printf("Sarakkeen %d arvo on %s:\n", column, targetValue);
        printNode(node, ); // Tulosta solmun sisältö
    } else {
        printf("Sarakkeen %d arvo ei ole %s, arvo oli: %s\n", column, targetValue, node->arvot[column]);
    }

    // Käy rekursiivisesti läpi puun vasen ja oikea lapsi
    printNodesWithColumnValue(node->left, sarakkeet, column, targetValue);
    printNodesWithColumnValue(node->right, sarakkeet, column, targetValue);
}








