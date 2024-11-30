#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "matrix.h"

int main(){
    matrix matr;
    int indicator = input(&matr);
    if (indicator == 0){return 0;}
    output(matr,1);
    ind_task(&matr);
    output(matr,2);
    clear(&matr);
    return 0;
}

// 1 случай максимальное кол-во отр в 1, максимальное кол-во пол в посл (1 своп) - легко решается
// 2 случай максимальное кол-во положит в посл, все отр не в 1 - ?
// 3 случай максимальное кол-во отр в первой, все пол не в посл - ?