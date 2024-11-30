#ifndef TREE_H
#define TREE_H

typedef struct Node {
    char* key;
    int info;
    struct Node* left;
    struct Node* right;
} Node;

#endif
