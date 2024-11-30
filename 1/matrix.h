#ifndef MATRIX
#define MATRIX

typedef struct Line{
    int nitems;
    int* vector;
} Line;

typedef struct matrix {
    int nlines;
    Line* masline;
} matrix;

void get_int(int* number);
void input(matrix* matr);
void output(matrix matr, int code);
void ind_task(matrix* matr);
void clear(matrix* matr);

#endif