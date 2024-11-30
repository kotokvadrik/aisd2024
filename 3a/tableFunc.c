#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "tableFunc.h"

char* myfilereadline(FILE* file){
    char *ptr = (char*)malloc(1);
    char buf[81];
    int n, len = 0;
    *ptr = '\0';
    do {
        n = fscanf(file,"%80[^\n]",buf);
        if (n < 0){
            free(ptr);
            ptr = NULL;
            continue;
        }
        if (n == 0){fscanf(file,"%*c");}
        else {
            len += strlen(buf);
            ptr = (char*) realloc(ptr,len + 1);
            strcat(ptr,buf);
        }
    } while(n > 0);
    return ptr;
}

int Add(Table* table, int key, int par, char* info){
    if(key < 0 || par < 0){return 3;}
    if(table == NULL){return 4;}
    int indicator = Find(table, key);
    if (indicator >= 0){return 1;}
    if (table -> csize >= table -> msize){return 2;}
    (table -> ks)[table -> csize].key = key;
    (table -> ks)[table -> csize].par = par;
    (table -> ks)[table -> csize].info = calloc(strlen(info)+1, sizeof(char));
    strcpy((table -> ks)[table -> csize].info, info);
    table -> csize += 1;
    return 0;
}

int Find(Table* table, int key){
    for(int i = 0; i < table -> csize; i++){
        if (key == (table -> ks)[i].key){return i;}
    }
    return -1;
}

int Delete(Table* table, int key){
    if (key < 0){return 2;}
    if (table == NULL){return 3;}
    int indicator = Find(table, key);
    if (indicator == -1){return 1;}
    
    free((table -> ks[indicator]).info);
    if (indicator != table -> csize - 1){
        table -> ks[indicator].key = table -> ks[table -> csize - 1].key;
        table -> ks[indicator].par = table -> ks[table -> csize - 1].par;
        (table -> ks[indicator]).info = calloc(strlen(table -> ks[table -> csize - 1].info) + 1, sizeof(char));
        strcpy(table -> ks[indicator].info, table -> ks[table -> csize - 1].info);
        free(table -> ks[table -> csize - 1].info);
    }
    table -> ks[table -> csize - 1].info = NULL;
    table -> csize -= 1;
    return 0;
}

int Import(Table** table, char* filename){
    FILE* file = fopen(filename,"r+t");
    if (file == NULL){return 1;}
    if (*table != NULL){eraseTable(table);}
    *table = calloc(1, sizeof(Table));
    (*table) -> msize = MAX_SIZE;
    (*table) -> csize = 0;
    (*table) -> ks = calloc(MAX_SIZE, sizeof(KeySpace));
    char* current_line = myfilereadline(file);
    char* word;
    int i = 0;
    while(current_line != NULL){
        if((*table) -> csize < (*table) -> msize){
            word = strtok(current_line, " \t");
            (*table) -> ks[i].key = atoi(word); 
            word = strtok(NULL, " \t");
            (*table) -> ks[i].par = atoi(word); 
            word = strtok(NULL, " \t");
            (*table) -> ks[i].info = calloc(strlen(word)+1, sizeof(char));
            strcpy((*table) -> ks[i].info, word);
            i++;
            (*table) -> csize ++;
        }
        free(current_line);
        current_line = myfilereadline(file);
    }
    fclose(file);
    return 0;
}

int Show(Table* table){
    if(table == NULL){return 2;}
    printf("Table:\n");
    int n_lines = table -> csize;
    if(n_lines <= 0){return 1;}
    for(int i = 0; i < n_lines; i++){
        printf("Key: %d Par: %d Info: \"%s\"\n", table -> ks[i].key, table -> ks[i].par, table -> ks[i].info);
    }
    return 0;
}

int DelParent(Table* table, int key){
    if(table == NULL){return 3;}
    if(key < 0){return 2;}
    if (Delete(table, key) == 1){return 1;}
    for(int i = 0; i < table -> csize; i++){
        if(table -> ks[i].par == key){table -> ks[i].par = 0;}
    }
    return 0;
}

int FindFamily(Table** table, int key){
    if(*table == NULL){return 4;}
    if(key < 0){return 3;}
    int parent_index = Find(*table, key);
    if(parent_index == -1){return 1;}
    Table* new_table = calloc(1, sizeof(Table));
    new_table -> msize = (*table) -> msize;
    new_table -> csize = 0;
    new_table -> ks = calloc(new_table -> msize, sizeof(KeySpace));
    printf("Parent:\nKey: %d Par: %d Info: \"%s\"\n", (*table) -> ks[parent_index].key, (*table) -> ks[parent_index].par, (*table) -> ks[parent_index].info);
    int indic;
    for(int i = 0; i < (*table) -> csize; i++){
        if((*table) -> ks[i].par == key){
            indic = Add(new_table, (*table) -> ks[i].key,(*table) -> ks[i].par, (*table) -> ks[i].info);
        }
    }
    eraseTable(table);
    *table = new_table;
    printf("Family:\n");
    indic = Show(*table);
    return 0;
}

void eraseTable(Table** table){
    if(*table != NULL){
        for(int i = 0; i < (*table) -> csize; i++){
            free(((*table) -> ks)[i].info);
        }
        free((*table) -> ks);
        free(*table);
        *table = NULL;
    }
}