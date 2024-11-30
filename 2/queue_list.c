#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

typedef struct dec {
    data* head;
    data* tail;
} dec;

typedef struct data{
        char* id;
        int ta;
        int ts;
        data* next;
} data;

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

dec* input(){
        dec* result = calloc(1, sizeof(dec));
    printf("Enter the input data: ");
    char* input_str = myreadline();
    data value;
    value.id = NULL; value.next = NULL;
   // dec result;
    result -> head = NULL; result -> tail = NULL;
    char* save_ptr1; char* save_ptr2; char* item;
    char* word = strtok_r(input_str, " ", &save_ptr1);
    while (word != NULL){
        item = strtok_r(word, "/", &save_ptr2);
        value.id = realloc(value.id, (strlen(item)+1) * sizeof(char));
        strcpy(value.id, item);
        item = strtok_r(NULL, "/", &save_ptr2); // atoi
        value.ta = atoi(item);
        item = strtok_r(NULL, "/", &save_ptr2); // atoi
        value.ts = atoi(item);
        put_tail(value, result); // положили 1 "value" - человека в очередь
        word = strtok_r(NULL, " ", &save_ptr1);
    }
    free(value.id);
    free(input_str);
    return result;
}

void put_tail(data elem, dec* queue){
    if (queue -> tail){
        (queue -> tail)-> next = calloc(1, sizeof(data));
        ((queue -> tail)-> next) -> ta = elem.ta;
        ((queue -> tail)-> next) -> ts = elem.ts;
        ((queue -> tail)-> next) -> id = calloc(strlen(elem.id) + 1, sizeof(char));
        (queue -> tail) = (queue -> tail) -> next;
        strcpy((queue -> tail) -> id, elem.id);
        (queue -> tail) -> next = NULL;
    }
    else{
        data* first = calloc(1, sizeof(data));
        queue -> head = first; queue -> tail = first;
        first -> ta = elem.ta; first -> ts = elem.ts;
        first -> id = calloc(strlen(elem.id) + 1, sizeof(char));
        strcpy(first -> id, elem.id);
        first -> next = NULL;
    }
}

data pop_head(dec* queue){
    data result;
    if (queue -> head == NULL) {
        printf("Reading error: queue is empty! We return empty result.\n");
        return result;
    }
    result = *(queue -> head);
    if (queue -> head == queue -> tail){
        free(queue -> head);
        queue -> head = NULL;
        queue -> tail = NULL;
        return result;
    }
    data* ptr = queue -> head;
    queue -> head = (queue -> head) -> next;
    free(ptr);
    return result;
}

void put_head(data elem, dec* queue){
    if (!(queue -> head)){
        put_tail(elem, queue);
        return;
    }
    data* ptr = (queue -> head);
    data* new_elem = calloc(1, sizeof(data));
    new_elem -> ta = elem.ta; new_elem -> ts = elem.ts;
    new_elem -> next = ptr;
    new_elem -> id = calloc(strlen(elem.id)+1, sizeof(char));
    strcpy(new_elem -> id, elem.id);
    (queue -> head) = new_elem;
}

data pop_tail(dec* queue){
    data result;
    if (queue -> tail == NULL){
        printf("Reading error: queue is empty! We return empty result.\n");
        return result;
    }
    if (queue -> head == queue -> tail){
        return pop_head(queue);
    }
    result = *(queue -> tail);
    data* memory = queue -> tail;
    queue -> tail = NULL;
    data* ptr = queue -> head;
    while ((ptr -> next) != memory){
        ptr = ptr -> next;
    }
    free(memory);
    queue -> tail = ptr;
    queue -> tail -> next = NULL;
    printf("TAIL AFTER POP TAIL: %s\n", ptr -> id);
    return result;
}

void event_generate(dec* queue, mas_event* time_mas){
    data* start = queue -> head;
    time_mas -> kolvo = 0;
    time_mas -> mas = NULL;
    while(start){
        (time_mas -> kolvo) += 1;
        time_mas -> mas = realloc(time_mas -> mas, (time_mas -> kolvo) * sizeof(event));
        (time_mas -> mas)[(time_mas -> kolvo) - 1].time = start -> ta;
        (time_mas -> mas)[(time_mas -> kolvo) - 1].code = 1;
        start = start -> next;
    }
    start = queue -> head; int ts_time;
    int t = start -> ta;
    while(start){
        (time_mas -> kolvo) += 1;
        time_mas -> mas = realloc(time_mas -> mas, (time_mas -> kolvo) * sizeof(event));
        ts_time = start -> ts;
        (time_mas -> mas)[(time_mas -> kolvo) - 1].time = ts_time + t;
        (time_mas -> mas)[(time_mas -> kolvo) - 1].code = 2;
        qsort(time_mas -> mas, time_mas -> kolvo, sizeof(event), compTimeVozr);
       // t = ts_time + t;
        t = (time_mas -> mas)[(time_mas -> kolvo) - 1].time;
        start = start -> next;
    }
}

int compTimeVozr(const void * a, const void * b){
        const event* first = (const event*) a;
        const event* second = (const event*) b;
        return (first->time - second->time);
}

void output_queue(dec* queue, int code){
    data* start = queue -> head;
    printf("Queue: ");
    while(start){
        if (code == 1){printf("%s/%d/%d ", start -> id, start -> ta, start -> ts);}
        if (code == 2){printf("%s ", start -> id);}
        start = start -> next;
    }
    printf("\n");
}

void output_item_of_queue(data elem){
        printf("\nValue: %s/%d/%d\n", elem.id, elem.ta, elem.ts);
}

void output_events(mas_event mas){
    printf("\nTimeLine: ");
    for (int i = 0; i < mas.kolvo; i++){
        printf("(%d,%d) ", mas.mas[i].time, mas.mas[i].code);
    }
    printf("\n");
}

void erase_events(mas_event* mas){
    free(mas -> mas);
}

void erase_queue(dec* queue){
    data* start = queue -> head;
    while(start){
        free(start -> id);
        data* ptr = start;
        start = start -> next;
        free(ptr);
        ptr = NULL;
    }
}

void process(dec* queue, mas_event* time_mas){
    int current_time; int code; int next_time; int flag; data item; data buffer;
    dec res_queue; res_queue.head = NULL; res_queue.tail = NULL;
    for (int i = 0; i < (time_mas -> kolvo); i++){
        flag = 1;
        current_time = (time_mas -> mas)[i].time;
        if ((i + 1) < (time_mas -> kolvo)){
            next_time = (time_mas -> mas)[i + 1].time;
            if (next_time == current_time){
                flag = 0;
            }
        } // flag == 1 - вывод, иначе - нет
        code = (time_mas -> mas)[i].code; // 1 - приход 2 - уход
        if (code == 1){
            item = pop_head(queue);
            if (item.id[0] == '*'){
                if ((res_queue.head -> id) != NULL){
                    buffer = pop_head(&res_queue);
                    put_head(item, &res_queue);
                    put_head(buffer, &res_queue);
                    free(buffer.id);
                }
                else{
                    put_head(item, &res_queue);
                }
            }
            else{put_tail(item, &res_queue);}
            free(item.id);
        }
        if (code == 2){
            item = pop_head(&res_queue);
            free(item.id);
        }
        if (flag == 1){
            printf("Time: %d\n", current_time);
            output_queue(&res_queue, 2);
        }
    }
    erase_queue(&res_queue);
}