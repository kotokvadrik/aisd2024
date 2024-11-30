#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "dialog.h"
#include "tableFunc.h"

int main(){
    Hashtable* table = NULL;
    int rc;
    const char* msgs[] = {"0.Quit", "1.Add", "2.Find (key)", "3.Delete (key)", "4.Show", "5.Import from .txt", "6.Output in .txt", "7.Find (key + release)", "8.Delete (key + release)", "9.Keyboard input"};
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    int (*fptr[])(Hashtable**) = {NULL, D_Add, D_Find, D_Delete, D_Show, D_Import, D_Output, D_FindRel, D_DeleteRel, D_KeyboardInit};
    while(rc = dialog(msgs, NMsgs)){
        if (!fptr[rc](&table)){
            break;
        }
    }
    printf("\nThat's all, bye.\n");
    eraseTable(&table);
    return 0;
}