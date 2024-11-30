#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "matrix.h"

int get_int(int* number, int code){ // code == 1 - kolvo, code == 0 - chislo
    int indicator;
    do{
        indicator = scanf("%d", number);
        if (((indicator == 0) && (code == 0)) || ((indicator == 1) && (code == 1) && (*number <= 0))){
            printf("Error: this is not a number. Try again\n");
            scanf("%*[^\n]");
            scanf("%*c");
        }
    } while(((indicator == 0) && (code == 0)) || ((indicator == 1) && (code == 1) && (*number <= 0)));
    if (indicator == 1){return 1;}
    if (indicator == -1){return 0;}
}

int input(matrix* matr){
    matr -> nlines = 0;
    printf("Enter the lines number\n");
    int indicator = get_int(&(matr -> nlines), 1);
    if (indicator == 0){
        return 0;
    }
    matr -> masline = calloc(matr -> nlines, sizeof(Line));
    for (int i = 0; i < matr -> nlines; i++){
        (matr -> masline)[i].nitems = 0;
    }
    for (int i = 0; i < matr -> nlines; i++){
        printf("This is the %d line. Please, enter the items number.\n", i+1);
        indicator = get_int(&(((matr -> masline)[i]).nitems), 1);
        if (indicator == 0){
            clear(matr);
            return 0;
        }
        ((matr -> masline)[i]).vector = calloc(((matr -> masline)[i]).nitems, sizeof(int));
        printf("We are ready to get the %d line.\n", i+1);
        for (int j = 0; j < (((matr -> masline)[i]).nitems); j++){
            indicator = get_int(&((((matr -> masline)[i]).vector)[j]),0);
            if (indicator == 0){
                clear(matr);
                return 0;
            }
        }
    }
    return 1;
}

void output(matrix matr, int code){
    Line current_line;
    if (code == 1){printf("\nInput matrix:\n");}
    else if (code == 2){printf("Output matrix:\n");}
    for(int i = 0; i < matr.nlines; i++){
        current_line = (matr.masline)[i];
        for(int j = 0; j < current_line.nitems; j++){
            printf("%d ", (current_line.vector)[j]);
        }
        printf("\n");
    }
    printf("\n");
}

void ind_task(matrix* matr){
    int kolvo_otr, kolvo_pol, line_otr_index, line_pol_index, maximum_pol = 0, maximum_otr = 0;
    Line current_line;
    for (int i = 0; i < matr -> nlines; i++){
        kolvo_otr = 0; kolvo_pol = 0;
        current_line = (matr -> masline)[i];
        for (int j = 0; j < current_line.nitems; j++){
            if ((current_line.vector)[j] > 0){kolvo_pol += 1;}
            else if ((current_line.vector)[j] < 0){kolvo_otr += 1;}
        }
        if (kolvo_pol >= maximum_pol){maximum_pol = kolvo_pol; line_pol_index = i;}
        if (kolvo_otr >= maximum_otr){maximum_otr = kolvo_otr; line_otr_index = i;}
    } // после цикла получены индексы нужных строк
    maximum_pol = ((matr -> masline)[line_pol_index]).nitems; // переопределение переменных максимум и минимум 
    maximum_otr = ((matr -> masline)[line_otr_index]).nitems; // в количество элементов в соответствующих строках
    
    int* buffer_pol = calloc(maximum_pol, sizeof(int)); // выделение буферных массивов для замены строк
    int* buffer_otr = calloc(maximum_otr, sizeof(int));
    
    for(int i = 0; i < maximum_pol; i++){ // заполнение этих буферных массивов, их можно было заполнить и данными 1/последней строки, без разницы для замены
        buffer_pol[i] = ((matr -> masline)[line_pol_index]).vector[i];
    }
    for(int i = 0; i < maximum_otr; i++){
        buffer_otr[i] = ((matr -> masline)[line_otr_index]).vector[i];
    }
    
    // заполнение строки положительных чисел элементами нулевой строки
    ((matr -> masline)[line_pol_index]).vector = realloc(((matr -> masline)[line_pol_index]).vector, sizeof(int)*(((matr -> masline)[0]).nitems));
    ((matr -> masline)[line_pol_index]).nitems = ((matr -> masline)[0]).nitems;
    for(int i = 0; i < ((matr -> masline)[line_pol_index]).nitems; i++){
        ((matr -> masline)[line_pol_index]).vector[i] = ((matr -> masline)[0]).vector[i];
    }
    // заполнение нулевой строки элементами строки положительных чисел
    ((matr -> masline)[0]).vector = realloc(((matr -> masline)[0]).vector, sizeof(int)*(maximum_pol));
    ((matr -> masline)[0]).nitems = maximum_pol;
    for(int i = 0; i < ((matr -> masline)[0]).nitems; i++){
        ((matr -> masline)[0]).vector[i] = buffer_pol[i];
    }
    // заполнение строки отрицательных чисел элементами последней строки
    ((matr -> masline)[line_otr_index]).vector = realloc(((matr -> masline)[line_otr_index]).vector, sizeof(int)*(((matr -> masline)[(matr -> nlines)-1]).nitems));
    ((matr -> masline)[line_otr_index]).nitems = ((matr -> masline)[(matr -> nlines)-1]).nitems;
    for(int i = 0; i < ((matr -> masline)[line_otr_index]).nitems; i++){
        ((matr -> masline)[line_otr_index]).vector[i] = ((matr -> masline)[(matr -> nlines)-1]).vector[i];
    }
    // заполнение последней строки элементами строки отрицательных чисел
    ((matr -> masline)[(matr -> nlines)-1]).vector = realloc(((matr -> masline)[(matr -> nlines)-1]).vector, sizeof(int)*(maximum_otr));
    ((matr -> masline)[(matr -> nlines)-1]).nitems = maximum_otr;
    for(int i = 0; i < ((matr -> masline)[(matr -> nlines)-1]).nitems; i++){
        ((matr -> masline)[(matr -> nlines)-1]).vector[i] = buffer_otr[i];
    }
    free(buffer_otr);
    free(buffer_pol);
}

void clear(matrix* matr){
    Line current_line;
    for(int i = 0; i < matr -> nlines; i++){
        current_line = (matr -> masline)[i];
        if ((current_line.nitems) > 0){
            free(current_line.vector);
        }
    }
    if ((matr -> nlines) > 0){
        free(matr -> masline);
    }
    matr -> nlines = 0;
    matr -> masline = NULL;
}