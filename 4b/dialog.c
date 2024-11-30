#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dialog.h"
#include "tree.h"
#include "treeFunc.h"

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

int D_Add(Node** tree, Node* ELIST){
    const char* errmsgs[] = {"Done.", "ERROR - no one dublicate", "Tree is not exist ._."};
    int key;
    int rc, info, n;
    int indic;
    printf("Enter key: --> ");
    n = getInt(&key);
    if(n == 0){return 0;} // неккоректный ввод
    printf("Enter info: --> ");
    n = getInt(&info);
    if(n == 0){return 0;} // неккоректный ввод
    indic = Add(tree, key, info, ELIST);
    if(indic == 0){rc = 0; printf("%s\n", errmsgs[rc]); return 1;}
    if(indic == 1){rc = 1; printf("%s\n", errmsgs[rc]); return 1;}
}

/*
D_Add, D_Find, D_Delete, D_Show, D_Import, 
D_Output, D_KeyboardInit, D_FindMin, D_TreeTraversal, D_Timing, D_makedot
*/

int D_Find(Node** tree, Node* ELIST){
    const char* errmsgs[] = {"Not in tree", "In tree", "Incorrect key", "Tree is not exist ._."};
    Node* rc;
    int key, n;
    if(*tree == NULL){printf("%s\n", errmsgs[3]); return 1;}
    printf("Enter key: --> ");
    n = getInt(&key);
    if(n == 0){return 0;} // неккоректный ввод
    rc = Find(*tree, key, ELIST);
    if(rc == NULL){printf("%s: %d\n", errmsgs[0], key); return 1;}
    printf("%s: key - %d, info - %d, color - %c\n", errmsgs[1], key, rc->info, rc->color);
    return 1;
}

int D_FindMin(Node** tree, Node* ELIST){
    const char* errmsgs[] = {"Not in tree", "Min", "Incorrect key", "Tree is not exist ._."};
    Node* rc;
    if(*tree == NULL){ printf("%s\n", errmsgs[3]); return 1;}
    rc = FindMin(*tree, ELIST);
    printf("%s: key - %d, info - %d, color - %c\n", errmsgs[1], rc->key, rc->info, rc->color);
    return 1;
}

int D_Delete(Node** tree, Node* ELIST){
    const char* errmsgs[] = {"Done.", "This element is not in tree", "Incorrect key", "Tree is not exist ._."};
    int rc;
    int key, n;
    if(*tree == NULL){printf("%s\n", errmsgs[3]); return 1;}
    printf("Enter key: --> ");
    n = getInt(&key);
    if(n == 0){return 0;} // неккоректный ввод
    Delete(tree, key, ELIST);
    printf("%s: %d\n", errmsgs[0], key);
    return 1;
}

int D_Show(Node** tree, Node* ELIST){
    if(*tree == NULL){printf("Tree is not exist :(\n"); return 1;}
    const char* errmsgs[] = {"Done."};
    Show(*tree, 0, ELIST);
    printf("%s\n", errmsgs[0]);
    return 1;
}

int D_Import(Node** tree, Node* ELIST){
    const char* errmsgs[] = {"Done.", "The file doesn't exist:("};
    char* filename = NULL;
    printf("Enter filename (+.txt): --> ");
    filename = myreadline();
    if (filename == NULL){return 0;} // неккоректный ввод
    FILE* f = fopen(filename, "r+t");
    Import(tree, f, ELIST);
    free(filename);
    fclose(f);
    printf("%s\n", errmsgs[0]);
    return 1;
}

int D_Output(Node** tree, Node* ELIST){
    const char* errmsgs[] = {"Done.", "The file doesn't exist:("};
    char* filename = NULL;
    if(*tree == NULL){printf("Tree is not exist :(\n"); return 1;}
    printf("Enter filename (+.txt): --> ");
    filename = myreadline();
    if (filename == NULL){return 0;} // неккоректный ввод
    FILE* f = fopen(filename, "w+t");
    Output(*tree, f, ELIST);
    free(filename);
    fclose(f);
    printf("%s\n", errmsgs[0]);
    return 1;
}

int D_TreeTraversal(Node** tree, Node* ELIST){
    if(*tree == NULL){printf("Tree is not exist :(\n"); return 1;}
    printf("Traversal (invers): ");
    TreeTraversal(*tree, ELIST);
    printf("\nDone.\n");
    return 1;
}

int D_makedot(Node** tree, Node* ELIST){
    if(*tree == NULL){printf("Tree is not exist :(\n"); return 1;}
    makedot(*tree, ELIST);
    return 1;
}

int D_Timing(Node** tree, Node* ELIST){
    printf("Enter the number of nodes: ");
    int n;
    int indic = getInt(&n);
    if(n == 0){return 0;}
    Timing(tree, n, ELIST);
    printf("Done.\n");
    return 1;
}

int D_Dop(Node** tree, Node* ELIST){
    eraseTree(tree, ELIST);
    printf("Enter filename (+.txt): --> ");
    char* filename = myreadline();
    if(filename == NULL){return 0;}
    FILE* f = fopen(filename, "r+t");
    char* line = myfilereadline(f);
    int counter = 1; int chislo, k;
    while(line != NULL){
        char* word = strtok(line, " ,");
        while(word != NULL){
            chislo = atoi(word);
            k = Add(tree, chislo, counter, ELIST);
            word = strtok(NULL, " ,");
        }
        free(line);
        line = myfilereadline(f);
        counter ++;
    }
    fclose(f);
    char itog[50] = "end_";
    strcat(itog, filename);
    free(filename);
    f = fopen(itog, "w+t");
    output_in_dop(*tree, f, ELIST);
    fclose(f);
    printf("Done.\n");
    return 1;
}
