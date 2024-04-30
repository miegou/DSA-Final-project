#ifndef AVL_H
#define AVL_H

#include "csv_reader.h"

// Node struct
typedef struct Node {
    char arvot[MAX_SARAKKEET][MAX_RIVI_PITUUS]; // Arvot kullekin sarakkeelle
    struct Node *left;
    struct Node *right;
    int height;
} Node;

struct Node *newNode(Rivi *rivi, Sarake *sarakkeet, int sarakkeiden_lkm);


struct Node *rightRotate(struct Node *y);
struct Node *leftRotate(struct Node *x);
int height(struct Node *N);
int max(int a, int b);
int getBalance(struct Node *N);
struct Node *insertNode(struct Node *node, Rivi *rivi, Sarake *sarakkeet, int sarakkeiden_lkm, char *vertailuarvo);
void printPreOrder(struct Node *root);
struct Node *minValueNode(struct Node *node);
struct Node *deleteNode(struct Node *root, char nimi[MAX_RIVI_PITUUS]);
void freeAVL(struct Node *node);
void printNode(struct Node *node, Sarake *sarakkeet);
void printNodesWithColumnValue(struct Node *node, int column, char *targetValue);



#endif
