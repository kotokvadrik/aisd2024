#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "dialog.h"
#include "treeFunc.h"
#include <time.h>

int main(){
    Node* tree = NULL;
    int rc;
    const char* msgs[] = {"0.Quit", "1.Add", "2.Find", "3.Delete", "4.Show", "5.Import from .txt", "6.Output in .txt", "7.Find min", "8.Tree traversal", "9.Timing", "10.Make .dot file", "11.Dop"};
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    int (*fptr[])(Node**) = {NULL, D_Add, D_Find, D_Delete, D_Show, D_Import, D_Output, D_FindMin, D_TreeTraversal, D_Timing, D_makedot, D_Dop};
    while(rc = dialog(msgs, NMsgs)){
        if (!fptr[rc](&tree)){
            break;
        }
    }
    printf("\nThat's all, bye.\n");
    eraseTree(&tree);
    return 0;
}
