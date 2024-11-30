#ifndef QUEUE_H
#define QUEUE_H

typedef struct data{
    char* name;
    data* next;
} data;

typedef struct Queue{
    data** head;
    data** tail;
} Queue;

void push_back(Queue* queue, char* item);
Queue* queue_init();
char* pop_top(Queue* queue);
void erase_queue(Queue** queue);

#endif