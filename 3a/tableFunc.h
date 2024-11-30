#ifndef TABLEFUNC_H
#define TABLEFUNC_H

int Add(Table* table, int key, int par, char* info);
int Find(Table* table, int key);
int Delete(Table* table, int key);
int Show(Table* table);
int Import(Table** table, char* filename);
int DelParent(Table* table, int key);
int FindFamily(Table** table, int key); // я так понимаю стереть старую таблицу и ответ - новая
void eraseTable(Table** table);
char* myfilereadline(FILE* file);

#endif