#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void push_back(Queue* queue, char* item){
    data* ptr = *(queue->tail);
    if(ptr == NULL){
        *(queue->head) = calloc(1, sizeof(data));
        (*(queue->head))->name = item;
        (*(queue->head))->next = NULL;
        *(queue->tail) = *(queue->head);
        return;
    }
    ptr = calloc(1, sizeof(data));
    ptr->name = item;
    ptr->next = NULL;
    (*(queue->tail))->next = ptr;
    *(queue->tail) = ptr;
}

Queue* queue_init(){
    Queue* queue = calloc(1, sizeof(Queue));
    *(queue->head) = NULL;
    *(queue->tail) = NULL;
    return queue;
}

char* pop_top(Queue* queue){
    if(*(queue->head) == NULL){return NULL;}
    char* res = (*(queue->head))->name;
    data* ptr = (*(queue->head));
    (*(queue->head)) = ptr->next;
    if(ptr == (*(queue->tail))){(*(queue->tail)) = NULL;}
    free(ptr);
    return res;
}

void erase_queue(Queue** queue){
    data* ptr = (*((*queue)->head));
    data* pred = NULL;
    while(ptr != NULL){
        pred = ptr;
        ptr = ptr->next;
        free(pred->name);
        free(pred);
    }
    if(*queue != NULL){
        free(*queue);
        *queue = NULL;
    }
}