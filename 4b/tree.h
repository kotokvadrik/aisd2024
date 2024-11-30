#ifndef TREE_H
#define TREE_H

typedef struct Node {
    int key;
    int info;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    char color;
} Node;

#endif
