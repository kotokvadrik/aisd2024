#ifndef DIALOG_H
#define DIALOG_H
#include "table.h"

int getInt(int* number);
int dialog(const char* msgs[], int N);
int D_Add(Hashtable**); 
int D_Find(Hashtable**);
int D_Delete(Hashtable**); 
int D_Show(Hashtable**);
int D_Import(Hashtable**);
int D_Output(Hashtable**);
int D_FindRel(Hashtable**);
int D_DeleteRel(Hashtable**);
char* myreadline();

#endif