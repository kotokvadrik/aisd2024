#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "dialog.h"
#include "graphFunc.h"

int main(){
    Vertex* vertex_list = NULL;
    int rc;
    const char* msgs[] = {"0.Quit", "1.Add_Vertex", "2.Add_Edge", "3.Delete_Vertex", "4.Delete_Edge", "5.Show_matrix", "6.Change_Vertex", "7.Change_Edge", "8.GraphViz", "9.Ostov", "10.Deikstra", "11.BFS", "12.Dop"};
    const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
    int (*fptr[])(Vertex**) = {NULL, D_Add_Vertex, D_Add_Edge, D_Delete_Vertex, D_Delete_Edge, D_Show_matrix, D_Change_Vertex, D_Change_Edge, D_GraphViz, D_Ostov, D_Deikstra, D_BFS, D_Dop};
    while(rc = dialog(msgs, NMsgs)){
        if (!fptr[rc](&vertex_list)){
            break;
        }
    }
    printf("\nThat's all, bye.\n");
    eraseGraph(&vertex_list);
    return 0;
}
