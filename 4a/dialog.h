#ifndef DIALOG_H
#define DIALOG_H
#include "tree.h"

int getInt(int* number);
int dialog(const char* msgs[], int N);
int D_Add(Node**); 
int D_Find(Node**);
int D_Delete(Node**); 
int D_Show(Node**);
int D_Import(Node**);
int D_Output(Node**);
int D_FindMin(Node**);
int D_TreeTraversal(Node**);
int D_Timing(Node**);
int D_makedot(Node**);
int D_Dop(Node**);
char* myreadline();

#endif
