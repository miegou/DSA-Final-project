#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

// Function to create a new AVL tree node
struct Node *newNode(char *nimi) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(node->nimi, nimi);
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

// Function to calculate height of a node
int height(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Max function
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to get balance factor of a node
int getBalance(struct Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Function to insert a node into AVL tree
struct Node *insertNode(struct Node *node, char nimi[MAX_RIVI_PITUUS]) {
    // Find the correct position to insert the node and insert it
    if (node == NULL)
        return (newNode(nimi));

    int compare = strcmp(nimi, node->nimi);
    if (compare < 0)
        node->left = insertNode(node->left, nimi);
    else if (compare > 0)
        node->right = insertNode(node->right, nimi);
    else
        return node;

    // Update the balance factor of each node and
    // Balance the tree
    node->height = 1 + max(height(node->left),
                   height(node->right));

    int balance = getBalance(node);
    if (balance > 1 && strcmp(nimi, node->left->nimi) < 0)
        return rightRotate(node);

    if (balance < -1 && strcmp(nimi, node->right->nimi) > 0)
        return leftRotate(node);

    if (balance > 1 && strcmp(nimi, node->left->nimi) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(nimi, node->right->nimi) < 0) {
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

// Function to delete a node from AVL tree
struct Node *deleteNode(struct Node *root, char nimi[MAX_RIVI_PITUUS]) {
    // Find the node and delete it
    if (root == NULL)
        return root;

    int compare = strcmp(nimi, root->nimi);
    if (compare < 0)
        root->left = deleteNode(root->left, nimi);
    else if (compare > 0)
        root->right = deleteNode(root->right, nimi);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Node *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            struct Node *temp = minValueNode(root->right);

            strcpy(root->nimi, temp->nimi);

            root->right = deleteNode(root->right, temp->nimi);
        }
    }

    if (root == NULL)
        return root;

    // Update the balance factor of each node and
    // balance the tree
    root->height = 1 + max(height(root->left),
                   height(root->right));

    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Function to print AVL tree in preorder traversal
void printPreOrder(struct Node *root) {
    if (root != NULL) {
        printf("%s ", root->nimi);
        printPreOrder(root->left);
        printPreOrder(root->right);
    }
}
