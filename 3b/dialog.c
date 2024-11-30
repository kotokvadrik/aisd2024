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

int D_Add(Hashtable** table){
    const char* errmsgs[] = {"Done.", "Incorrect key", "Table is not exist ._.", "Incorrect value"};
    int key, info, rc;
    if(*table == NULL){rc = 2; printf("%s\n", errmsgs[rc]); return 1;}
    printf("Enter key: --> ");
    int n = getInt(&key);
    if(n == 0){return 0;} // неккоректный ввод
    printf("Enter info: --> ");
    n = getInt(&info);
    if(n == 0){return 0;} // неккоректный ввод
    rc = Add(table, key, info);
    printf("%s: %d\n", errmsgs[rc], key);
    return 1;
}

int D_Find(Hashtable** table){
    const char* errmsgs[] = {"Not in table", "In table", "Incorrect key", "Table is not exist ._."};
    int key, rc;
    if(*table == NULL){rc = 3; printf("%s\n", errmsgs[rc]); return 1;}
    printf("Enter key: --> ");
    int n = getInt(&key);
    if(n == 0){return 0;} // неккоректный ввод
    rc = Find(*table, key);
    printf("%s: %d\n", errmsgs[rc], key);
    return 1;
}

int D_Delete(Hashtable** table){
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

int D_Show(Hashtable** table){
    const char* errmsgs[] = {"Done.", "The table is empty.", "Table is not exist ._."};
    int rc = Show(*table);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_Import(Hashtable** table){
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

int D_Output(Hashtable** table){
    const char* errmsgs[] = {"Done."};
    char* filename = NULL;
    printf("Enter filename (+.txt): --> ");
    filename = myreadline();
    if (filename == NULL){return 0;} // неккоректный ввод
    int rc = Output(*table, filename);
    free(filename);
    printf("%s\n", errmsgs[rc]);
    return 1;
}

int D_FindRel(Hashtable** table){
    const char* errmsgs[] = {"Not in table", "In table", "Incorrect key", "Table is not exist ._.", "Incorrect release"};
    int key, rc, release;
    if(*table == NULL){rc = 3; printf("%s\n", errmsgs[rc]); return 1;}
    printf("Enter key: --> ");
    int n = getInt(&key);
    if(n == 0){return 0;} // неккоректный ввод
    printf("Enter release: --> ");
    n = getInt(&release);
    if(n == 0){return 0;} // неккоректный ввод
    rc = FindRel(*table, key, release);
    printf("%s: %d, release %d\n", errmsgs[rc], key, release);
    return 1;
}

int D_DeleteRel(Hashtable** table){
    const char* errmsgs[] = {"Done.", "This element is not in table", "Incorrect key", "Table is not exist ._.", "Incorrect release"};
    int key, rc, release;
    if(*table == NULL){rc = 3; printf("%s\n", errmsgs[rc]); return 1;}
    printf("Enter key: --> ");
    int n = getInt(&key);
    if(n == 0){return 0;} // неккоректный ввод
    printf("Enter release: --> ");
    n = getInt(&release);
    if(n == 0){return 0;} // неккоректный ввод
    rc = DeleteRel(*table, key, release);
    printf("%s: %d, release %d\n", errmsgs[rc], key, release);
    return 1;
}

int D_KeyboardInit(Hashtable** table){
    if(*table != NULL){eraseTable(table);}
    const char* errmsgs[] = {"Done.", "Duplicate key, this line is not in table", "Incorrect key, this line is not in table"};
    int key, par, rc, msize, nlines;
    char* information; char* line;
    *table = calloc(1, sizeof(Hashtable));
    (*table) -> csize = 0;

    printf("Enter msize: --> ");
    int n = getInt(&msize);
    while(n != 0 && msize <= 0){
        printf("Thats incorrect msize (must be > 0)\n");
        n = getInt(&msize);
    }
    if(n == 0){return 0;} // EOF
    (*table) -> msize = msize;
    (*table)->table = calloc(msize, sizeof(KeySpace*));

    for(int i = 0; i < msize; i++){
        (*table)->table[i] = NULL;
    }

    printf("Enter lines of table number: --> ");
    n = getInt(&nlines);
    while(n != 0 && nlines < 0){
        printf("Thats incorrect number (must be >= 0)\n");
        n = getInt(&nlines);
    }
    if(n == 0){return 0;} // EOF

    printf("Enter the table (lines) with format key_info:\n");
    for(int i = 0; i < nlines; i++){
        line = myreadline();
        if(line == NULL){return 0;} // EOF
        information = strtok(line, " \t");
        key = atoi(information);
        information = strtok(NULL, " \t");
        par = atoi(information);
        rc = Add(table, key, par);
        free(line);
    }
    printf("%s\n", errmsgs[0]);
    return 1;
}