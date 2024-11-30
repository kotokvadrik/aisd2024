#ifndef TREEFUNC_H
#define TREEFUNC_H
#include "tree.h"

Node* Add(Node** tree, char* key, int info); //
Node* Find(Node* tree, char* key, int code); //
void Delete(Node** tree, char* key); //
void Show(Node* ptr, int level); //
void Import(Node** tree, FILE* f); //
void Output(Node* tree, FILE* f); //
int KeyboardInit(Node** tree);
Node* FindMin(Node* tree); //
void TreeTraversal(Node* tree); //
void Timing(Node** tree, int n); //
void makedot(Node* tree); //
void output_in_dot(Node* tree, FILE* f); //
void eraseTree(Node** tree); //
char* myfilereadline(FILE* file); //
void output_in_dop(Node* tree, FILE* f); //

#endif
