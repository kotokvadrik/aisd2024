#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "dialog.h"
#include "tableFunc.h"

int main(){
    Table* table = NULL;
    int rc;
    const char* msgs[] = {"0.Quit", "1.Add", "2.Find", "3.Delete", "4.Show", "5.Import from .txt", "6.Delete parent key", "7.Find family", "8.Keyboard input"};
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    int (*fptr[])(Table**) = {NULL, D_Add, D_Find, D_Delete, D_Show, D_Import, D_DelParent, D_FindFamily, D_KeyboardInit}; 
    while(rc = dialog(msgs, NMsgs)){
        if (!fptr[rc](&table)){
            break;
        }
    }
    printf("\nThat's all, bye.\n");
    eraseTable(&table);
    return 0;
}