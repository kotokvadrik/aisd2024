#ifndef MAIN_H
#define MAIN_H
#define MAX_NUM 3

typedef struct dec dec;

typedef struct data data;

typedef struct event{
    int time;
    int code; // 1 - приход, 2 - уход
} event;

typedef struct mas_event{
    int kolvo;
    event* mas;
} mas_event;

void put_tail(data elem, dec* queue); // elem - void
data pop_head(dec* queue); // void*
void put_head(data elem, dec* queue); // elem - void
data pop_tail(dec* queue); // void*
void output_item_of_queue(data elem);
void output_queue(dec* queue, int code);
int compTimeVozr(const void * a, const void * b);
char* myreadline();
dec* input(int* indicator);
void event_generate(dec* queue, mas_event* time_mas);
void output_events(mas_event mas);
void erase_events(mas_event* mas);
void erase_queue(dec* queue);
void process(dec* queue, mas_event* time_mas);

#endif