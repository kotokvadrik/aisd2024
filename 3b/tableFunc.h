#ifndef TABLEFUNC_H
#define TABLEFUNC_H
#include "table.h"

unsigned long long hash(int key);
int Add(Hashtable**, int key, int info);
int Find(Hashtable* table, int key);
int Delete(Hashtable* table, int key);
int Show(Hashtable* table);
int Import(Hashtable** table, char* filename);
int Output(Hashtable* table, char* filename);
int FindRel(Hashtable* table, int key, int release);
int DeleteRel(Hashtable* table, int key, int release);
int D_KeyboardInit(Hashtable** table);
void eraseTable(Hashtable** table);
char* myfilereadline(FILE* file);
void Extend(Hashtable** table);
int RunningCounter(Hashtable* table);

#endif