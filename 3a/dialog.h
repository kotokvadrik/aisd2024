#ifndef DIALOG_H
#define DIALOG_H

int getInt(int* number);
int dialog(const char* msgs[], int N);
int D_Add(Table**); 
int D_Find(Table**);
int D_Delete(Table**); 
int D_Show(Table**);
int D_Import(Table**);
int D_DelParent(Table**); 
int D_FindFamily(Table**);
int D_KeyboardInit(Table**);
char* myreadline();

#endif