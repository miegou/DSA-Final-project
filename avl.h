#ifndef AVL_H
#define AVL_H

#include "csv_reader.h"

struct Node {
    char nimi[MAX_RIVI_PITUUS];
    struct Node *left;
    struct Node *right;
    int height;
};

struct Node *newNode(char *nimi);
struct Node *rightRotate(struct Node *y);
struct Node *leftRotate(struct Node *x);
int height(struct Node *N);
int max(int a, int b);
int getBalance(struct Node *N);
struct Node *insertNode(struct Node *node, char nimi[MAX_RIVI_PITUUS]);
void printPreOrder(struct Node *root);
struct Node *minValueNode(struct Node *node);
struct Node *deleteNode(struct Node *root, char nimi[MAX_RIVI_PITUUS]);
void freeAVL(struct Node *node);
void printNode(struct Node *node);

#endif
