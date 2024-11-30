#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dialog.h"
#include "table.h"
#include "tableFunc.h"

int getInt(int* number){
    int indicator;
    do{
        indicator = scanf("%d", number);
        if (indicator == 0){
            printf("Error: this is not a number. Try again\n");
            scanf("%*[^\n]");
            scanf("%*c");
        }
    } while(indicator == 0);
    scanf("%*c");
    if (indicator == 1){return 1;}
    if (indicator == -1){return 0;}
}

int dialog(const char* msgs[], int N){
    char* errmsg = "";
    int rc;
    int i, n;
    do{
        puts(errmsg);
        errmsg = "You are wrong. Repeat, please!";
        for(i = 0; i < N; ++i){
            puts(msgs[i]);
        }
        puts("Make your choice: --> ");
        n = getInt(&rc); 
        if(n == 0){
            rc = 0;
        }
    } while(rc < 0 || rc >= N);
    return rc;
} // rc == 0 - EOF or 0

char* myreadline(){
    char *ptr = (char*)malloc(1);
    char buf[81];
    int n, len = 0;
    *ptr = '\0';
    do {
        n = scanf("%80[^\n]",buf);
        if (n < 0){
            free(ptr);
            ptr = NULL;
            continue;
        }
        if (n == 0){scanf("%*c");}
        else {
            len += strlen(buf);
            ptr = (char*) realloc(ptr,len + 1);
            strcat(ptr,buf);
        }
    } while(n > 0);
    return ptr;
}

int D_Add(Table** table){
    const char* errmsgs[] = {"Done.", "Duplicate key", "Table overflow", "Incorrect key", "Table is not exist ._."};
    int key, par, rc;
    if(*table == NULL){rc = 4; printf("%s\n", errmsgs[rc]); return 1;}
    printf("Enter key: --> ");
    int n = getInt(&key);
    if(n == 0){return 0;} // неккоректный ввод
    printf("Enter par: --> ");
    n = getInt(&par);
    if(n == 0){return 0;} // неккоректный ввод
    printf("Enter info: ");
    char* information = NULL;
    information = myreadline();
    if (information == NULL){return 0;} // неккоректный ввод
    rc = Add(*table, key, par, information);
    free(information);
    printf("%s: %d\n", errmsgs[rc], key);
    return 1;
}

int D_Find(Table** table){
    const char* errmsgs[] = {"Not in table", "In table", "Incorrect key", "Table is not exist ._."};
    int key, rc;
    if(*table == NULL){rc = 3; printf("%s\n", errmsgs[rc]); return 1;}
    printf("Enter key: --> ");
    int n = getInt(&key);
    if(n == 0){return 0;} // неккоректный ввод
    if(key < 0){rc = 2; printf("%s: %d\n", errmsgs[rc], key); return 1;}
    rc = Find(*table, key);
    if (rc == -1){rc = 0;}
    else if(rc != -1){rc = 1;}
    printf("%s: %d\n", errmsgs[rc], key);
    return 1;
}

int D_Delete(Table** table){
    const char* errmsgs[] = {"Done.", "This element is not in table", "Incorrect key", "Table is not exist ._."};
    int key, rc;
    if(*table == NULL){rc = 3; printf("%s\n", errmsgs[rc]); return 1;}
    printf("Enter key: --> ");
    int n = getInt(&key);
    if(n == 0){return 0;} // неккоректный ввод
    rc = Delete(*table, key);
    printf("%s: %d\n", errmsgs[rc], key);
    return 1;
}

int D_Show(Table** table){
    const char* errmsgs[] = {"Done.", "The table is empty.", "Table is not exist ._."};
    int rc = Show(*table);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Import(Table** table){
    const char* errmsgs[] = {"Done.", "The file doesn't exist:("};
    char* filename = NULL;
    printf("Enter filename (+.txt): --> ");
    filename = myreadline();
    if (filename == NULL){return 0;} // неккоректный ввод
    int rc = Import(table, filename);
    free(filename);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_DelParent(Table** table){
    const char* errmsgs[] = {"Done.", "This element is not in table", "Incorrect key", "Table is not exist ._."};
    int key, rc;
    if(*table == NULL){rc = 3; printf("%s\n", errmsgs[rc]); return 1;}
    printf("Enter key: --> ");
    int n = getInt(&key);
    if(n == 0){return 0;} // неккоректный ввод
    rc = DelParent(*table, key);
    printf("%s: %d\n", errmsgs[rc], key);
    return 1;
}

int D_FindFamily(Table** table){
    const char* errmsgs[] = {"Done.", "This element is not in table", "Lonely parent", "Incorrect key", "Table is not exist ._."};
    int key, rc;
    if(*table == NULL){rc = 4; printf("%s\n", errmsgs[rc]); return 1;}
    printf("Enter key: --> ");
    int n = getInt(&key);
    if(n == 0){return 0;} // неккоректный ввод
    rc = FindFamily(table, key);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_KeyboardInit(Table** table){
    if(*table != NULL){eraseTable(table);}
    const char* errmsgs[] = {"Done.", "Duplicate key, this line is not in table", "Table overflow, this line is not in table", "Incorrect key, this line is not in table"};
    int key, par, rc, msize, nlines;
    char* information; char* line;
    *table = calloc(1, sizeof(Table));
    (*table) -> csize = 0;
    (*table) -> ks = calloc(1, sizeof(KeySpace));

    printf("Enter msize: --> ");
    int n = getInt(&msize);
    while(n != 0 && msize <= 0){
        printf("Thats incorrect msize (must be > 0)\n");
        n = getInt(&msize);
    }
    if(n == 0){return 0;} // EOF
    (*table) -> msize = msize;
    (*table) -> ks = realloc((*table) -> ks, msize * sizeof(KeySpace));

    printf("Enter lines of table number: --> ");
    n = getInt(&nlines);
    while(n != 0 && nlines < 0){
        printf("Thats incorrect number (must be >= 0)\n");
        n = getInt(&nlines);
    }
    if(n == 0){return 0;} // EOF

    printf("Enter the table (lines) with format key_par_info:\n");
    for(int i = 0; i < nlines; i++){
        line = myreadline();
        if(line == NULL){return 0;} // EOF
        information = strtok(line, " \t");
        key = atoi(information);
        information = strtok(NULL, " \t");
        par = atoi(information);
        information = strtok(NULL, " \t");
        rc = Add(*table, key, par, information);
        if(rc == 1 || rc == 2 || rc == 3){printf("%s\n", errmsgs[rc]);}
        free(line);
    }
    printf("%s\n", errmsgs[0]);
    return 1;
}

// везде где ключи - проверка ключа, и ещё table на NULL внутри tableFunc