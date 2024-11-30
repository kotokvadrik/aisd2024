#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(){
        int indicator = 0;
    dec* queue = input(&indicator);
    if (indicator == 1){
        erase_queue(queue);
        free(queue);
        return 0;
    }
    mas_event time_mas;
    event_generate(queue, &time_mas);
    qsort(time_mas.mas,time_mas.kolvo,sizeof(event),compTimeVozr);
    output_queue(queue, 1);
    output_events(time_mas);
    process(queue, &time_mas);
    erase_events(&time_mas);
    erase_queue(queue);
    free(queue);
    return 0;
}