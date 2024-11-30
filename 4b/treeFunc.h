#ifndef TREEFUNC_H
#define TREEFUNC_H
#include "tree.h"

int Add(Node** tree, int key, int info, Node* ELIST); //
Node* Find(Node* tree, int key, Node* ELIST); //
void Delete(Node** tree, int key, Node* ELIST); //
void Show(Node* ptr, int level, Node* ELIST); //
void Import(Node**tree, FILE* f, Node* ELIST); //
void Output(Node* tree, FILE* f, Node* ELIST); //
Node* FindMin(Node* tree, Node* ELIST); //
Node* FindMax(Node* tree, Node* ELIST); //
void TreeTraversal(Node* tree, Node* ELIST); //
void Timing(Node** tree, int n, Node* ELIST);
void makedot(Node* tree, Node* ELIST); //
void output_in_dot(Node* tree, FILE* f, Node* ELIST); //
void eraseTree(Node** tree, Node* ELIST); //
char* myfilereadline(FILE* file); // 
void output_in_dop(Node* tree, FILE* f, Node* ELIST); //
void add_balance(Node* ptr, Node** tree, Node* ELIST);//
void del_balance(Node* ptr, Node** tree, Node* ELIST);//
void left_rotate(Node* ptr, Node* ELIST); //
void right_rotate(Node* ptr, Node* ELIST); //

#endif
