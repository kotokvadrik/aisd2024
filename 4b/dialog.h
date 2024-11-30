#ifndef DIALOG_H
#define DIALOG_H
#include "tree.h"

int getInt(int* number);
int dialog(const char* msgs[], int N);
int D_Add(Node**, Node*); 
int D_Find(Node**, Node*);
int D_Delete(Node**, Node*); 
int D_Show(Node**, Node*);
int D_Import(Node**, Node*);
int D_Output(Node**, Node*);
int D_FindMin(Node**, Node*);
int D_TreeTraversal(Node**, Node*);
int D_Timing(Node**, Node*);
int D_makedot(Node**, Node*);
int D_Dop(Node**, Node*);
char* myreadline();

#endif
