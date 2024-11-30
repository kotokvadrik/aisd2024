#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

unsigned long long hash(int key){
    int digit; unsigned long long res = 1;
    while(key > 0){
        digit = key % 10;
        res = 37 * res + digit;
        key /= 10;
    }
    return abs(res);
}

// вектор Кейспэйс*, список Кейспэйс, список Ноуд

int Add(Hashtable** table, int key, int info){
    if(key < 0){return 1;}
    if(info < 0){return 3;}
    if (RunningCounter(*table)){Extend(table);}
    int ind = hash(key) % ((*table) -> msize);
    if (((*table) -> table)[ind] == NULL){ // под массив Keyspace* выделена память
        ((*table) -> table)[ind] = calloc(1, sizeof(KeySpace));
        (((*table) -> table)[ind])->count_release = 1;
        (((*table) -> table)[ind])->key = key;
        (((*table) -> table)[ind])->next = NULL;
        (((*table) -> table)[ind])->node = calloc(1, sizeof(Node));
        (((*table) -> table)[ind])->node->next = NULL;
        (((*table) -> table)[ind])->node->info = info;
        (((*table) -> table)[ind])->node->release = 1;
        (*table)->csize += 1;
        return 0;
    }
    KeySpace* current_line = ((*table) -> table)[ind];
    Node* current_release_line;
    while(current_line != NULL){
        if (current_line -> key == key){
            current_line->count_release += 1;
            current_release_line = current_line->node;
            Node* new_release = calloc(1, sizeof(Node));
            new_release->info = info;
            new_release->next = current_release_line;
            new_release->release = current_line->count_release;
            current_line->node = new_release;
            return 0;
        }
        current_line = current_line->next;
    }
    current_line = ((*table) -> table)[ind];
    KeySpace* new_keyspace = calloc(1,sizeof(KeySpace));
    new_keyspace->count_release = 1;
    new_keyspace->key = key;
    new_keyspace->next = current_line;
    new_keyspace->node = calloc(1,sizeof(Node));
    new_keyspace->node->next = NULL;
    new_keyspace->node->release = 1;
    new_keyspace->node->info = info;
    ((*table) -> table)[ind] = new_keyspace;
    return 0;
}

int Find(Hashtable* table, int key){
    if(key < 0){return 2;}
    int ind = hash(key) % (table -> msize);
    KeySpace* current_line = (table -> table)[ind];
    if(current_line == NULL){return 0;}
    while(current_line != NULL){
        if(current_line->key == key){
            printf("Key: %d, count releases: %d\n", key, current_line->count_release);
            Node* current_release_line = current_line->node;
            while(current_release_line != NULL){
                printf("Release: %d, info: %d\n", current_release_line->release, current_release_line->info);
                current_release_line = current_release_line->next;
            }
            return 1;
        }
        current_line = current_line->next;
    }
    return 0;
}

int Delete(Hashtable* table, int key){
    if(key < 0){return 2;}
    int ind = hash(key) % (table -> msize);
    KeySpace* current_line = (table -> table)[ind];
    if(current_line == NULL){return 1;}
    Node* current_release_line; Node* ptr;
    if(current_line -> next == NULL && current_line->key == key){
        current_release_line = current_line->node;
        while(current_release_line != NULL){
            ptr = current_release_line;
            current_release_line = current_release_line->next;
            free(ptr);
        }
        free(current_line);
        (table -> table)[ind] = NULL;
        table->csize -= 1;
        return 0;
    }
    if(current_line-> key == key){
        current_release_line = current_line->node;
        while(current_release_line != NULL){
                ptr = current_release_line;
                current_release_line = current_release_line->next;
                        free(ptr);
        }
        (table -> table)[ind] = current_line->next;
        free(current_line);
        return 0;
    }
    KeySpace* prev = (table -> table)[ind];
    while(current_line != NULL){
        if(current_line->key == key){
            current_release_line = current_line->node;
            while(current_release_line != NULL){
                ptr = current_release_line;
                current_release_line = current_release_line->next;
                free(ptr);
            }
            prev->next = current_line->next;
            free(current_line);
            return 0;
        }
        prev = current_line;
        current_line = current_line->next;
    }
    return 1;
}

int Show(Hashtable* table){
    if(table == NULL){return 2;}
    printf("Table:\n");
    KeySpace* current_line;
    for(int i = 0; i < table->msize; i++){
        if((table->table)[i] == NULL){printf("Keyspace %d: NULL\n", i+1); continue;}
        current_line = (table->table)[i];
        printf("Keyspace %d: ", i+1);
        while(current_line != NULL){
            printf("key %d -> ", current_line->key);
            current_line = current_line->next;
        }
        printf("NULL\n");
    }
    printf("\n");
    Node* current_release_line;
    for(int i = 0; i < table->msize; i++){
        if((table->table)[i] == NULL){printf("Keyspace %d: NULL\n", i+1); continue;}
        current_line = (table->table)[i];
        printf("Keyspace %d:\n", i+1);
        while(current_line != NULL){
            printf("    key %d: ", current_line->key);
            current_release_line = current_line->node;
            while(current_release_line != NULL){
                printf("release %d, info %d -> ", current_release_line->release, current_release_line->info);
                current_release_line = current_release_line->next;
            }
            printf("NULL\n");
            current_line = current_line->next;
        }
    }
    return 0;
}

int FindRel(Hashtable* table, int key, int release){
    if(key < 0){return 2;}
    if(release < 1){return 4;}
    int ind = hash(key) % (table -> msize);
    KeySpace* current_line = (table -> table)[ind];
    if(current_line == NULL){return 0;}
    while(current_line != NULL){
        if(current_line->key == key){
            Node* current_release_line = current_line->node;
            while(current_release_line != NULL){
                if(current_release_line->release == release){
                    printf("Key: %d Release: %d Info: %d\n", key, release, current_release_line->info);
                    return 1;
                }
                current_release_line = current_release_line->next;
            }
        }
        current_line = current_line->next;
    }
    return 0;
}

int DeleteRel(Hashtable* table, int key, int release){
    if (key < 0){return 2;}
    if (release < 1){return 4;}
    int ind = hash(key) % (table -> msize);
    KeySpace* current_line = (table -> table)[ind];
    if(current_line == NULL){return 1;}
    int indic;
    while (current_line != NULL){
        if (current_line->key == key && release == 1 && current_line->count_release == 1){indic = Delete(table,key); return 0;}
        if (current_line->key == key){
            Node* current_release_line = current_line->node;
            Node* prev;
            if(current_release_line->release == release){
                prev = current_release_line;
                current_line->node = current_release_line->next;
                free(prev);
                return 0;
            }
            while(current_release_line != NULL){
                if(current_release_line->release == release){
                    prev->next = current_release_line->next;
                    free(current_release_line);
                    return 0;
                }
                prev = current_release_line;
                current_release_line = current_release_line->next;
            }
        }
        current_line = current_line->next;
    }
    return 1;
}

void eraseTable(Hashtable** table){
    if(*table != NULL){
        KeySpace* current_line;
        Node* current_release_line; Node* ptr; KeySpace* pointer;
        for(int i = 0; i < (*table) -> msize; i++){
            if((*table)->table[i] == NULL){continue;}
            current_line = (*table)->table[i];
            while(current_line != NULL){
                pointer = current_line;
                current_release_line = current_line->node;
                while(current_release_line != NULL){
                    ptr = current_release_line;
                    current_release_line = current_release_line->next;
                    free(ptr);
                }
                current_line = current_line->next;
                free(pointer);
            }
        }
        free((*table) -> table);
        free(*table);
        *table = NULL;
    }
}

int Import(Hashtable** table, char* filename){
    FILE* file = fopen(filename,"r+t");
    if (file == NULL){return 1;}
    if (*table != NULL){eraseTable(table);}
    *table = calloc(1, sizeof(Hashtable));
    (*table) -> msize = MAX_SIZE;
    (*table) -> csize = 0;
    (*table) -> table = calloc(MAX_SIZE, sizeof(KeySpace*));
    for(int i = 0; i < MAX_SIZE; i++){
        (*table)->table[i] = NULL;
    }
    char* current_line = myfilereadline(file);
    char* word;
    int i = 0;
    int key; int info; int indic;
    while(current_line != NULL){
        word = strtok(current_line, " \t");
        key = atoi(word); 
        word = strtok(NULL, " \t");
        info = atoi(word);
        indic = Add(table,key,info);
        free(current_line);
        current_line = myfilereadline(file);
    }
    fclose(file);
    return 0;
}

int Output(Hashtable* table, char* filename){return 0;}

void Extend(Hashtable** table){
    Hashtable* new_table = calloc(1, sizeof(Hashtable));
    new_table->msize = ((*table)->msize)*2;
    new_table->csize = 0;
    new_table->table = calloc(new_table->msize, sizeof(KeySpace*));
    for(int i = 0; i < new_table->msize; i++){
        new_table->table[i] = NULL;
    }
    KeySpace* current_line;
    Node* current_relise_line; int key; int indic;
    for(int i = 0; i < (*table)->msize; i++){
        current_line = (*table)->table[i];
        while(current_line != NULL){
            key = current_line->key;
            current_relise_line = current_line->node;
            while(current_relise_line != NULL){
                indic = Add(&new_table, key, current_relise_line->info);
                current_relise_line = current_relise_line->next;
            }
            current_line = current_line->next;
        }
    }
    eraseTable(table);
    *table = new_table;
}

int RunningCounter(Hashtable* table){ // 1 - extend, 0 - no
    int counter = 0; int k;
    KeySpace* current_line;
    for(int i = 0; i < table->msize; i++){
        if(table->table[i] == NULL){continue;}
        k = 1;
        current_line = table->table[i];
        while(current_line != NULL){
            k++;
            current_line = current_line->next;
        }
        if(k > counter){counter = k;}
    }
    if(counter >= (table->msize)){return 1;}
    return 0;
}