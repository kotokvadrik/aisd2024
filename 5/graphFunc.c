#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "dialog.h"
#include "graphFunc.h"
#include "queue.h"
#include <limits.h>

#define WHITE 0
#define GREY 1
#define BLACK 2
#define INF INT_MAX

Vertex* find_vertex(Vertex* vertex_list, int port, int* index, char* name, int code){
    Vertex* ptr = vertex_list;
    int i = 0;
    while(ptr != NULL){
        if(code == 1 && port == ptr->port){return ptr;}
        if(code == 2 && (*index) == i){return ptr;}
        if(code == 3 && strcmp(ptr->name, name) == 0){return ptr;}
        if(code == 4 && strcmp(ptr->name, name) == 0){*index = i; return ptr;}
        ptr = ptr->next;
        i++;
    }
    return NULL;
}

void add_vertex(Vertex** vertex_list, char* name, int port){
    Vertex* ptr = *vertex_list;
    if(ptr == NULL){
        *vertex_list = calloc(1, sizeof(Vertex));
        (*((*vertex_list)->edge_list)) = NULL;
        (*vertex_list)->name = calloc(strlen(name)+1, sizeof(char));
        strcpy((*vertex_list)->name, name);
        (*vertex_list)->port = port;
        (*vertex_list)->next = NULL;
        return;
    }
    while(ptr->next != NULL){
        ptr = ptr->next;
    }
    Vertex* pred = ptr;
    ptr = ptr->next;
    ptr = calloc(1, sizeof(Vertex));
    (*(ptr->edge_list)) = NULL;
    ptr->name = calloc(strlen(name)+1, sizeof(char));
    strcpy(ptr->name, name);
    ptr->port = port;
    ptr->next = NULL;
    pred->next = ptr;
}

Edge* find_edge(Edge* edge_list, Vertex* vertex_to, Edge** pred_find_edge){
    Edge* ptr = edge_list;
    Edge* pred = NULL;
    while(ptr != NULL){
        if(vertex_to == ptr->vertex_to){return ptr;}
        pred = ptr;
        ptr = ptr->next;
    }
    *pred_find_edge = pred;
    return NULL;
}

void add_edge(Edge** edge_list, Vertex* vertex_to, int delay){
    Edge* pred_find_edge;
    Edge* nt = find_edge(*edge_list, vertex_to, &pred_find_edge);
    if(pred_find_edge == NULL){
        *edge_list = calloc(1, sizeof(Edge));
        (*edge_list)->vertex_to = vertex_to;
        (*edge_list)->delay = delay;
        (*edge_list)->next = NULL;
        for(int i = 0; i < MAX_VERTEX; i++){
            ((*edge_list)->port_access)[i] = 0;
        }
        return;
    }
    Edge* new_edge = calloc(1, sizeof(Edge));
    new_edge->delay = delay;
    new_edge->next = NULL;
    for(int i = 0; i < MAX_VERTEX; i++){
        (new_edge->port_access)[i] = 0;
    }
    new_edge->vertex_to = vertex_to;
    pred_find_edge->next = new_edge;
} // после добавления ребра нужно добавить зеркальное ребро в той вершине куда идет

void del_edge(Edge** edge_list, Vertex* vertex_to){
    Edge* pred_find_edge;
    Edge* finded = find_edge(*edge_list, vertex_to, &pred_find_edge);
    if(finded == NULL){return;}
    if(pred_find_edge == NULL){
        free(*edge_list);
        *edge_list = NULL;
        return;
    }
    pred_find_edge->next = finded->next;
    free(finded);
} // после удаления ребра нужно удалить зеркальное ребро в той вершине куда идет

void del_vertex(Vertex** vertex_list, char* name){
    Vertex* deliting_v = find_vertex(*vertex_list, 0, NULL, name, 3);
    if (deliting_v == NULL){return;}
    Edge* ptr = *(deliting_v->edge_list);
    while(ptr != NULL){
        Vertex* v = ptr->vertex_to;
        del_edge(v->edge_list, deliting_v);
        ptr = ptr->next;
    }
    ptr = *(deliting_v->edge_list);
    Edge* pred;
    while(ptr != NULL){
        pred = ptr;
        ptr = ptr->next;
        free(pred);
    }
    *(deliting_v->edge_list) = NULL;
    free(deliting_v->name);
    Vertex* pointer = *vertex_list;
    while(pointer->next != deliting_v){
        pointer = pointer->next;
    }
    pointer->next = deliting_v->next;
    free(deliting_v);
}

void change_vertex_port(Vertex* vertex_list, char* name, int new_port){
    Vertex* v = find_vertex(vertex_list, 0, NULL, name, 3);
    if (v == NULL){return;}
    v->port = new_port;
}

void change_edge_delay(Edge* edge_list, Vertex* vertex_to, int new_delay){
    Edge* pred;
    Edge* e = find_edge(edge_list, vertex_to, &pred);
    if(e == NULL){return;}
    e->delay = new_delay;
}

void change_edge_port_access(Edge* edge_list, Vertex* vertex_to, int new_port){ // идея с -18 - убрать порт 18
    Edge* pred;
    Edge* e = find_edge(edge_list, vertex_to, &pred);
    if(e == NULL){return;}
    for(int i = 0; i < MAX_VERTEX; i++){
        if(e->port_access[i] == 0){e->port_access[i] = new_port; return;}
    }
    printf("THE EDGE HAS NO PLACE TO ADD NEW PORT\n");
    return;
}

void print_matrix(Vertex* vertex_list){
    Vertex* ptr = vertex_list;
    int nVertex = 0;
    char** vertex_names = calloc(MAX_VERTEX, sizeof(char*));
    while(ptr != NULL){
        vertex_names[nVertex] = ptr->name;
        ptr = ptr->next;
        nVertex++;
    }
    vertex_names = realloc(vertex_names, nVertex*sizeof(char*));
    printf("MATRIX\n\t\t");
    for(int i = 0; i < nVertex; i++){
        printf("%s ", vertex_names[i]);
    }
    printf("\n");
    Vertex* current_vertex;
    Edge* current_edge;
    Edge* pred;
    current_vertex = vertex_list;
    for(int i = 0; i < nVertex; i++){
        printf("%s\t", vertex_names[i]);
        for(int j = 0; j < nVertex; j++){
            current_edge = find_edge(*(current_vertex->edge_list), find_vertex(vertex_list, 0, &j, NULL, 2), &pred);
            if(current_edge != NULL){printf("%d\t", current_edge->delay);}
            else{printf("%d\t", 0);}
        }
        current_vertex = current_vertex->next;
        printf("\n");
    }
    free(vertex_names);
}

int call_index_of_vertex(char** vertex_names, char* name, int nVertex){
    for(int i = 0; i < nVertex; i++){
        if(strcmp(name, vertex_names[i]) == 0){return i;}
    }
}

void BFS(Vertex* vertex_list, char* name){
    Vertex* ptr = vertex_list;
    int nVertex = 0;
    char** vertex_names = calloc(MAX_VERTEX, sizeof(char*));
    while(ptr != NULL){
        vertex_names[nVertex] = ptr->name;
        ptr = ptr->next;
        nVertex++;
    }// mb ne nado
    vertex_names = realloc(vertex_names, nVertex*sizeof(char*)); // потом очистить
    Vertex* pred[nVertex];
    int distance[nVertex];
    int color[nVertex];
    for(int i = 0; i < nVertex; i++){
        pred[i] = NULL;
        distance[i] = INF;
        color[i] = WHITE;
    }
    int index_of_begin_BFS;
    Vertex* beginning_vertex = find_vertex(vertex_list, 0, &index_of_begin_BFS, name, 4);
    color[index_of_begin_BFS] = GREY;
    d[index_of_begin_BFS] = 0;
    Queue* queue = queue_init();
    char* name = calloc(strlen(beginning_vertex->name)+1, sizeof(char));
    strcpy(name, beginning_vertex->name);
    push_back(queue, name);
    Vertex* current_vertex;
    char* current_vertex_name, *smezhnaya_vertex_name;
    Vertex* smezhnaya_vertex;
    int index_of_current_vertex;
    while(*(queue->head) != NULL){
        current_vertex_name = pop_top(queue);
        current_vertex = find_vertex(vertex_list, 0, NULL, current_vertex_name, 3);
        index_of_current_vertex = call_index_of_vertex(vertex_names, current_vertex->name, nVertex);
        free(current_vertex_name);
        Edge* pointer = *(current_vertex->edge_list);
        while(pointer != NULL){
            smezhnaya_vertex = pointer->vertex_to;
            int index_of_smezh_vertex = call_index_of_vertex(vertex_names, smezhnaya_vertex->name, nVertex);
            if(color[index_of_smezh_vertex] == WHITE){
                color[index_of_smezh_vertex] = GREY;
                d[index_of_smezh_vertex] = d[index_of_current_vertex] + pointer->delay;
                pred[index_of_smezh_vertex] = current_vertex;
                smezhnaya_vertex_name = calloc(strlen(smezhnaya_vertex->name)+1, sizeof(char));
                strcpy(smezhnaya_vertex_name, smezhnaya_vertex->name);
                push_back(queue, smezhnaya_vertex_name);
            }
            pointer = pointer->next;
        }
        color[index_of_current_vertex] = BLACK;
    }
    free(vertex_names);
}