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

int D_Add(Node** tree){
    const char* errmsgs[] = {"Done.", "Old info is:", "Tree is not exist ._."};
    char* key;
    int rc, info, n;
    Node* indic;
    printf("Enter key: --> ");
    key = myreadline();
    if(key == NULL){return 0;} // неккоректный ввод
    printf("Enter info: --> ");
    n = getInt(&info);
    if(n == 0){return 0;} // неккоректный ввод
    indic = Add(tree, key, info);
    if(indic == NULL){rc = 0; printf("%s\n", errmsgs[rc]); free(key); return 1;}
    if(indic != NULL){rc = 1; printf("%s %d\n", errmsgs[rc], indic->info); free(indic->key); free(indic); free(key); return 1;}
}

/*
D_Add, D_Find, D_Delete, D_Show, D_Import, 
D_Output, D_KeyboardInit, D_FindMin, D_TreeTraversal, D_Timing, D_makedot
*/

int D_Find(Node** tree){
    const char* errmsgs[] = {"Not in tree", "In tree", "Incorrect key", "Tree is not exist ._."};
    Node* rc;
    char* key;
    if(*tree == NULL){ printf("%s\n", errmsgs[3]); return 1;}
    printf("Enter key: --> ");
    key = myreadline();
    if(key == NULL){return 0;} // неккоректный ввод
    rc = Find(*tree, key, 1);
    if(rc == NULL){printf("%s: %s\n", errmsgs[0], key); free(key); return 1;}
    printf("%s: key - %s, info - %d\n", errmsgs[1], key, rc->info);
    free(key);
    return 1;
}

int D_FindMin(Node** tree){
    const char* errmsgs[] = {"Not in tree", "Min", "Incorrect key", "Tree is not exist ._."};
    Node* rc;
    if(*tree == NULL){ printf("%s\n", errmsgs[3]); return 1;}
    rc = FindMin(*tree);
    printf("%s: key - %s, info - %d\n", errmsgs[1], rc->key, rc->info);
    return 1;
}

int D_Delete(Node** tree){
    const char* errmsgs[] = {"Done.", "This element is not in tree", "Incorrect key", "Tree is not exist ._."};
    int rc;
    char* key;
    if(*tree == NULL){printf("%s\n", errmsgs[3]); return 1;}
    printf("Enter key: --> ");
    key = myreadline();
    if(key == NULL){return 0;} // неккоректный ввод
    Delete(tree, key);
    printf("%s: %s\n", errmsgs[0], key);
    free(key);
    return 1;
}

int D_Show(Node** tree){
    if(*tree == NULL){printf("Tree is not exist :(\n"); return 1;}
    const char* errmsgs[] = {"Done."};
    Show(*tree, 0);
    printf("%s\n", errmsgs[0]);
    return 1;
}

int D_Import(Node** tree){
    const char* errmsgs[] = {"Done.", "The file doesn't exist:("};
    char* filename = NULL;
    printf("Enter filename (+.txt): --> ");
    filename = myreadline();
    if (filename == NULL){return 0;} // неккоректный ввод
    FILE* f = fopen(filename, "r+t");
    Import(tree, f);
    free(filename);
    fclose(f);
    printf("%s\n", errmsgs[0]);
    return 1;
}

int D_Output(Node** tree){
    const char* errmsgs[] = {"Done.", "The file doesn't exist:("};
    char* filename = NULL;
    if(*tree == NULL){printf("Tree is not exist :(\n"); return 1;}
    printf("Enter filename (+.txt): --> ");
    filename = myreadline();
    if (filename == NULL){return 0;} // неккоректный ввод
    FILE* f = fopen(filename, "w+t");
    Output(*tree, f);
    free(filename);
    fclose(f);
    printf("%s\n", errmsgs[0]);
    return 1;
}

int D_TreeTraversal(Node** tree){
    if(*tree == NULL){printf("Tree is not exist :(\n"); return 1;}
    printf("Traversal (invers): ");
    TreeTraversal(*tree);
    printf("Done.\n");
    return 1;
}

int D_makedot(Node** tree){
    if(*tree == NULL){printf("Tree is not exist :(\n"); return 1;}
    makedot(*tree);
    return 1;
}

int D_Timing(Node** tree){
    printf("Enter the number of nodes: ");
    int n;
    int indic = getInt(&n);
    if(n == 0){return 0;}
    Timing(tree, n);
    printf("Done.\n");
    return 1;
}

int D_Dop(Node** tree){
    eraseTree(tree);
    printf("Enter filename (+.txt): --> ");
    char* filename = myreadline();
    if(filename == NULL){return 0;}
    FILE* f = fopen(filename, "r+t");
    char* line = myfilereadline(f);
    char* resline = calloc(1,sizeof(char));
    int len = 0;
    while(line != NULL){
        resline = realloc(resline, (len+strlen(line)+2)*sizeof(char));
        strcat(resline, " ");
        strcat(resline, line);
        free(line);
        line = myfilereadline(f);
        len = strlen(resline);
    }
    char* word = strtok(resline, " ,.!?");
    Node* item, * finded;
    while(word != NULL){
        item = Add(tree, word, 1);
        if(item != NULL){
            finded = Find(*tree, word, 1);
            finded->info = item->info + 1;
            free(item->key); free(item);
        }
        word = strtok(NULL, " !?.,");
    }
    free(resline);
    fclose(f);
    char itog[50] = "end_";
    strcat(itog, filename);
    free(filename);
    f = fopen(itog, "w+t");
    output_in_dop(*tree, f);
    fclose(f);
    printf("Done.\n");
    return 1;
}
