#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

typedef struct data{
        char* id;
        int ta;
        int ts;
} data;

typedef struct dec{
    int head;
    int tail;
    data queue[MAX_NUM]; // void* queue for universal
} dec;

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

dec* input(int* indicator){
    dec* result = calloc(1,sizeof(dec));
    printf("Enter the input data: ");
    char* input_str = myreadline();
    data value;
    value.id = NULL;
    result -> head = 0; result -> tail = 0;
    for (int i = 0; i < MAX_NUM; i ++){
        ((result -> queue))[i].id = NULL;
    }
    char* save_ptr1; char* save_ptr2; char* item;
    char* word = strtok_r(input_str, " ", &save_ptr1);
    int mas_count = 0;
    while (word != NULL){
        if (mas_count == MAX_NUM){
                printf("Reading error: queue is full! Error.\n");
                *indicator = 1;
            break;
        }
        item = strtok_r(word, "/", &save_ptr2);
        value.id = realloc(value.id, (strlen(item)+1) * sizeof(char));
        strcpy(value.id, item);
        item = strtok_r(NULL, "/", &save_ptr2); // atoi
        value.ta = atoi(item);
        item = strtok_r(NULL, "/", &save_ptr2); // atoi
        value.ts = atoi(item);
        put_tail(value, result); // положили 1 "value" - человека в очередь
        mas_count += 1;
        word = strtok_r(NULL, " ", &save_ptr1);
    }
    free(value.id);
    free(input_str);
    return result;
}

void put_tail(data elem, dec* queue){
    if (((queue -> head) == (queue -> tail)) && ((queue -> queue)[queue -> tail].id != NULL)){
        printf("Reading error: queue is full! Error.\n");
        return;
    }
    ((*queue).queue)[(*queue).tail].ta = elem.ta;
    ((*queue).queue)[(*queue).tail].ts = elem.ts;
    (((*queue).queue)[(*queue).tail]).id = realloc((((*queue).queue)[(*queue).tail]).id,(strlen(elem.id) + 1) * sizeof(char));
    strcpy((((*queue).queue)[(*queue).tail]).id, elem.id);
    (*queue).tail = ((*queue).tail + 1) % MAX_NUM;
}

data pop_head(dec* queue){
    data result;
    if (((queue -> head) == (queue -> tail)) && ((queue -> queue)[queue -> head].id == NULL)){
        printf("Reading error: queue is empty! We return empty result.\n");
        return result;
    }
    int first = queue -> head;
    result = (queue -> queue)[first];
    char* buf = calloc(strlen(result.id) + 1, sizeof(char));
    strcpy(buf, result.id);
    free(result.id);
    result.id = buf;
    (queue -> queue)[first].id = NULL;
    queue -> head = (first + 1) % MAX_NUM;
    if ((queue -> head) == (queue -> tail)){
       // printf("Queue is empty. Operation was successfully.\n");
    }
    return result;
}

void put_head(data elem, dec* queue){
    if (((queue -> head) == (queue -> tail)) && ((queue -> queue)[queue -> head].id != NULL)){
        printf("Reading error: queue is full! Error.\n");
        return;
    }
    (queue -> head) -= 1;
    if ((queue -> head) < 0){
        (queue -> head) = MAX_NUM - 1;
    }
    (queue -> queue)[queue -> head].ta = elem.ta;
    (queue -> queue)[queue -> head].ts = elem.ts;
    (queue -> queue)[queue -> head].id = realloc((queue -> queue)[queue -> head].id, (strlen(elem.id) + 1) * sizeof(char));
    strcpy((queue -> queue)[queue -> head].id, elem.id);
}

data pop_tail(dec* queue){
    data result;
    if (((queue -> head) == (queue -> tail)) && ((queue -> queue)[queue -> tail].id == NULL)){
        printf("Reading error: queue is empty! We return empty result.\n");
        return result;
    }
    int last = queue -> tail;
    if (last - 1 < 0){
        last = MAX_NUM;
    }
    result = (queue -> queue)[last-1];
    char* buf = calloc(strlen(result.id) + 1, sizeof(char));
    strcpy(buf, result.id);
    free(result.id);
    result.id = buf;
    (queue -> queue)[last-1].id = NULL;
    last -= 1;
    queue -> tail = last;
    if ((queue -> head) == (queue -> tail)){
       // printf("Queue is empty. Operation was successfully.\n");
    }
    return result;
}

void event_generate(dec* queue, mas_event* time_mas){
    int start = queue -> head;
    time_mas -> kolvo = 0;
    time_mas -> mas = NULL;
    int num = 0;
    if (start == (queue -> tail)){
        for (int i = 0; i < MAX_NUM; i++){
                (time_mas -> kolvo) += 1;
                time_mas -> mas = realloc(time_mas -> mas, (time_mas -> kolvo) * sizeof(event));
                (time_mas -> mas)[(time_mas -> kolvo) - 1].time = ((queue -> queue)[i]).ta;
                (time_mas -> mas)[(time_mas -> kolvo) - 1].code = 1;
        }
    }
    else{
        while(start != (queue -> tail)){
                (time_mas -> kolvo) += 1;
                time_mas -> mas = realloc(time_mas -> mas, (time_mas -> kolvo) * sizeof(event));
                (time_mas -> mas)[(time_mas -> kolvo) - 1].time = ((queue -> queue)[start]).ta;
                (time_mas -> mas)[(time_mas -> kolvo) - 1].code = 1;
                start = (start + 1) % MAX_NUM;
            }
    }
    start = queue -> head; int ts_time;
    int t = ((queue -> queue)[start]).ta;
    if (start == (queue -> tail)){
        while(num != MAX_NUM){
                        (time_mas -> kolvo) += 1;
                time_mas -> mas = realloc(time_mas -> mas, (time_mas -> kolvo) * sizeof(event));
                ts_time = ((queue -> queue)[start]).ts;
                (time_mas -> mas)[(time_mas -> kolvo) - 1].time = ts_time + t;
                (time_mas -> mas)[(time_mas -> kolvo) - 1].code = 2;
                qsort(time_mas -> mas, time_mas -> kolvo, sizeof(event), compTimeVozr);
               // t = ts_time + t;
                t = (time_mas -> mas)[(time_mas -> kolvo) - 1].time;
                start = (start + 1) % MAX_NUM;
                num ++;
        }
    }
    else{
            while(start != (queue -> tail)){
                (time_mas -> kolvo) += 1;
                time_mas -> mas = realloc(time_mas -> mas, (time_mas -> kolvo) * sizeof(event));
                ts_time = ((queue -> queue)[start]).ts;
                (time_mas -> mas)[(time_mas -> kolvo) - 1].time = ts_time + t;
                (time_mas -> mas)[(time_mas -> kolvo) - 1].code = 2;
                qsort(time_mas -> mas, time_mas -> kolvo, sizeof(event), compTimeVozr);
               // t = ts_time + t;
                t = (time_mas -> mas)[(time_mas -> kolvo) - 1].time;
                start = (start + 1) % MAX_NUM;
            }
        }
}

int compTimeVozr(const void * a, const void * b){
        const event* first = (const event*) a;
        const event* second = (const event*) b;
        return (first->time - second->time);
}

void output_queue(dec* queue, int code){
    int start = queue -> head;
    printf("Queue: ");
    int num = 0;
    if(start == queue -> tail){
        while (num != MAX_NUM){
                if (code == 1){printf("%s/%d/%d ", (queue -> queue)[start].id, (queue -> queue)[start].ta, (queue -> queue)[start].ts);}
                if (code == 2){printf("%s ", (queue -> queue)[start].id);}
                start = (start + 1) % MAX_NUM;
                num ++;
            }
    }
    else{
        while(start != (queue -> tail)){
                if (code == 1){printf("%s/%d/%d ", (queue -> queue)[start].id, (queue -> queue)[start].ta, (queue -> queue)[start].ts);}
                if (code == 2){printf("%s ", (queue -> queue)[start].id);}
                start = (start + 1) % MAX_NUM;
            }
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
    int start = queue -> head;
    if(start == queue -> tail){
        for(int i = 0; i < MAX_NUM; i++){
                char* ptr = (queue -> queue)[i].id;
                free(ptr);
                ptr = NULL;
        }
    }
    else{
        while(start != (queue -> tail)){
                char* ptr = (queue -> queue)[start].id;
                free(ptr);
                ptr = NULL;
                start = (start + 1) % MAX_NUM;
            }
    }
}

void process(dec* queue, mas_event* time_mas){
    int current_time; int code; int next_time; int flag; data item; data buffer;
    dec res_queue; res_queue.head = 0; res_queue.tail = 0;
    for (int i = 0; i < MAX_NUM; i ++){
            res_queue.queue[i].id = NULL;
    }
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
                            if (res_queue.queue[0].id != NULL){
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
            if(flag == 1){
                    printf("Time: %d\n", current_time);
                    if(current_time == (time_mas -> mas)[(time_mas -> kolvo)-1].time){
                        printf("Queue: \n");
                    }
                    else{
                        output_queue(&res_queue, 2);
                    }
            }
    }
    erase_queue(&res_queue);
}