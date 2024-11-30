#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_int(int* number, int code){ // code == 1 - kolvo, code == 0 - chislo
    int indicator;
    do{
        indicator = scanf("%d", number);
        if (((indicator == 0) && (code == 0)) || ((indicator == 1) && (code == 1) && (*number <= 0))){
            printf("Error: this is not a number. Try again\n");
            scanf("%*[^\n]");
            scanf("%*c");
        }
        scanf("%*[^\n]");
        scanf("%*c");
    } while(((indicator == 0) && (code == 0)) || ((indicator == 1) && (code == 1) && (*number <= 0)));
    if (indicator == 1){return 1;}
    if (indicator == -1){return 0;}
} // 1 - всё хорошо, 0 - EOF

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

void recordBin(){
    int linecount; int nitems; int item;
    printf("Введите имя бинарного файла (+.bin): ");
    char * filename = myreadline();
    FILE* file = fopen(filename,"w+b");
    printf("Введите количество строк матрицы: ");
    int indicator = get_int(&linecount,1);
    if (indicator == 0){printf("Остановка ввода - EOF\n"); return;}
    fwrite(&linecount, sizeof(int), 1, file);
    for (int i = 0; i < linecount; i++){
        printf("Введите количество элементов %d строки: ", i+1);
        indicator = get_int(&nitems,1);
        if (indicator == 0){printf("Остановка ввода - EOF\n"); return;}
        fwrite(&nitems, sizeof(int), 1, file);
        printf("Введите %d строку: ", i+1);
        for (int j = 0; j < nitems; j++){
            indicator = get_int(&item,0);
            if (indicator == 0){printf("Остановка ввода - EOF\n"); return;}
            fwrite(&item, sizeof(int), 1, file);
        }
    }
    printf("\nInput matrix:\n");
    fseek(file, 1*sizeof(int), SEEK_SET);
    for (int i = 0; i < linecount; i++){
        fread(&nitems, sizeof(int), 1, file);
        for (int j = 0; j < nitems; j++){
            fread(&item, sizeof(int), 1, file);
            printf("%d ", item);
        }
        printf("\n");
    }
    printf("\n");
    fclose(file);
    free(filename);
}

int count_items_of_line(int* mas, int nitems, int code){ // code == 0 - pol, code == 1 - otr
    int pol = 0, otr = 0;
    for (int i = 0; i < nitems; i++){
        if (mas[i] > 0){pol ++;}
        else if (mas[i] < 0){otr ++;}
    }
    if (code == 0) {return pol;}
    if (code == 1) {return otr;}
}

void readingBin(){
    printf("Введите имя входного файла (+.bin): ");
    char* filename = myreadline();
    FILE* fileINPUT = fopen(filename,"r+b");
        if(fileINPUT != NULL){
            int* mas = NULL; int counter = 0;

            int nPol, nOtr, maxPOL = 0, maxOTR = 0, indPOL, indOTR, item;
            int linecount, nitems;
            fread(&linecount, sizeof(int), 1, fileINPUT);
            while (fread(&nitems, sizeof(int), 1, fileINPUT) == 1){
                nPol = 0, nOtr = 0;
                for (int i = 0; i < nitems; i++){
                    fread(&item,sizeof(int), 1, fileINPUT);
                    if (item > 0){nPol ++;}
                    else if (item < 0){nOtr ++;}
                }
               // read_Line_Of_Matrix(&mas, nitems);
               // nPol = count_items_of_line(mas, nitems, 0);
               // nOtr = count_items_of_line(mas, nitems, 1);
                if (nPol > maxPOL){maxPOL = nPol; indPOL = counter;}
                if (nOtr > maxOTR){maxOTR = nOtr; indOTR = counter;}
                counter++; // с помощью счетчика мы запомним нужную строку
            }
            // получили индексы 2х нужных строк
            printf("Введите имя выходного файла (+.bin): ");
            char* outputname = myreadline();
            FILE* fileOUTPUT = fopen(outputname, "w+b");
            fwrite(&linecount, sizeof(int), 1, fileOUTPUT); // записали количество строк
            fseek(fileINPUT,1 * sizeof(int), SEEK_SET);
            int currentN; int* mas_indOTR; int* mas_indPOL;

            fread(&currentN, sizeof(int), 1, fileINPUT);
            int* mas_FIRST_str = calloc(currentN+1, sizeof(int));
            mas_FIRST_str[0] = currentN;
            for (int i = 1; i < currentN+1; i++){
                fread(&(mas_FIRST_str[i]), sizeof(int), 1, fileINPUT);
            } // заполнили массив первой строки
            
            if((indOTR - indPOL) > 0){
                counter = 0;
                fseek(fileINPUT, 1 * sizeof(int), SEEK_SET);
                while (counter != indPOL){
                    fread(&currentN, sizeof(int), 1, fileINPUT);
                    fseek(fileINPUT, (currentN) * sizeof(int), SEEK_CUR);
                    counter ++;
                }
                fread(&currentN, sizeof(int), 1, fileINPUT); // записали кол-во элементов строки indPOL
                mas_indPOL = calloc(currentN+1, sizeof(int));
                mas_indPOL[0] = currentN;
                for (int i = 1; i < currentN+1; i++){
                    fread(&(mas_indPOL[i]), sizeof(int), 1, fileINPUT);
                } // заполнили массив строки indPOL
                counter++;
                
                while (counter != indOTR){
                    fread(&currentN, sizeof(int), 1, fileINPUT);
                    fseek(fileINPUT, (currentN) * sizeof(int), SEEK_CUR);
                    counter ++;
                }
                fread(&currentN, sizeof(int), 1, fileINPUT); // записали кол-во элементов строки indOTR
                mas_indOTR = calloc(currentN+1, sizeof(int));
                mas_indOTR[0] = currentN;
                for (int i = 1; i < currentN+1; i++){
                    fread(&(mas_indOTR[i]), sizeof(int), 1, fileINPUT);
                } // заполнили массив строки indOTR
                counter++;
            }
            
            else if((indOTR - indPOL) < 0){
                counter = 0;
                fseek(fileINPUT, 1 * sizeof(int), SEEK_SET);
                
                while (counter != indOTR){
                    fread(&currentN, sizeof(int), 1, fileINPUT);
                    fseek(fileINPUT, (currentN) * sizeof(int), SEEK_CUR);
                    counter ++;
                }
                fread(&currentN, sizeof(int), 1, fileINPUT); // записали кол-во элементов строки indOTR
                mas_indOTR = calloc(currentN+1, sizeof(int));
                mas_indOTR[0] = currentN;
                for (int i = 1; i < currentN+1; i++){
                    fread(&(mas_indOTR[i]), sizeof(int), 1, fileINPUT);
                } // заполнили массив строки indOTR
                counter++;
                while (counter != indPOL){
                    fread(&currentN, sizeof(int), 1, fileINPUT);
                    fseek(fileINPUT, (currentN) * sizeof(int), SEEK_CUR);
                    counter ++;
                }
                fread(&currentN, sizeof(int), 1, fileINPUT); // записали кол-во элементов строки indPOL
                mas_indPOL = calloc(currentN+1, sizeof(int));
                mas_indPOL[0] = currentN;
                for (int i = 1; i < currentN+1; i++){
                    fread(&(mas_indPOL[i]), sizeof(int), 1, fileINPUT);
                } // заполнили массив строки indPOL
                counter++;
            }

            while (counter != (linecount-1)){
                fread(&currentN, sizeof(int), 1, fileINPUT);
                fseek(fileINPUT, (currentN) * sizeof(int), SEEK_CUR);
                counter ++;
            }
            fread(&currentN, sizeof(int), 1, fileINPUT); // записали кол-во элементов последней строки
            int* mas_LAST_str = calloc(currentN+1, sizeof(int));
            mas_LAST_str[0] = currentN;
            for (int i = 1; i < currentN+1; i++){
                fread(&(mas_LAST_str[i]), sizeof(int), 1, fileINPUT);
            } // заполнили массив последней строки
            counter++;

            fseek(fileINPUT, 1 * sizeof(int), SEEK_SET); // вернулись в начало файла
            // counter = 0;
            for (int i = 0; i < linecount; i++){ // составление нового файла
                if (i == 0) { // записали строку с наибольшим количеством положительных членов в первую строку
                    fwrite(&(mas_indPOL[0]), sizeof(int), 1, fileOUTPUT);
                    for (int k = 1; k < mas_indPOL[0]+1; k++){
                        fwrite(&(mas_indPOL[k]), sizeof(int), 1, fileOUTPUT);
                    }
                    fread(&currentN, sizeof(int), 1, fileINPUT);
                    fseek(fileINPUT, (currentN) * sizeof(int), SEEK_CUR);
                }
                else if (i == indOTR) { // 
                    fwrite(&(mas_LAST_str[0]), sizeof(int), 1, fileOUTPUT);
                    for (int p = 1; p < mas_LAST_str[0]+1; p++){
                        fwrite(&(mas_LAST_str[p]), sizeof(int), 1, fileOUTPUT);
                    }
                    fread(&currentN, sizeof(int), 1, fileINPUT);
                    fseek(fileINPUT, (currentN) * sizeof(int), SEEK_CUR);
                }
                else if (i == indPOL) { // 
                    fwrite(&(mas_FIRST_str[0]), sizeof(int), 1, fileOUTPUT);
                    for (int l = 1; l < mas_FIRST_str[0]+1; l++){
                        fwrite(&(mas_FIRST_str[l]), sizeof(int), 1, fileOUTPUT);
                    }
                    fread(&currentN, sizeof(int), 1, fileINPUT);
                    fseek(fileINPUT, (currentN) * sizeof(int), SEEK_CUR);
                }
                else if (i == (linecount - 1)) { // 
                    fwrite(&(mas_indOTR[0]), sizeof(int), 1, fileOUTPUT);
                    for (int c = 1; c < mas_indOTR[0]+1; c++){
                        fwrite(&(mas_indOTR[c]), sizeof(int), 1, fileOUTPUT);
                    }
                    fread(&currentN, sizeof(int), 1, fileINPUT);
                    fseek(fileINPUT, (currentN) * sizeof(int), SEEK_CUR);
                }
                else{
                    fread(&currentN, sizeof(int), 1, fileINPUT);
                    fwrite(&(currentN), sizeof(int), 1, fileOUTPUT);
                    for (int j = 0; j < currentN; j++){
                        fread(&item, sizeof(int), 1, fileINPUT);
                        fwrite(&item, sizeof(int), 1, fileOUTPUT);
                    }
                }
            }
            printf("Output matrix:\n");
            fseek(fileOUTPUT, 1*sizeof(int), SEEK_SET);
            for (int i = 0; i < linecount; i++){
                fread(&nitems, sizeof(int), 1, fileOUTPUT);
                for (int j = 0; j < nitems; j++){
                    fread(&item, sizeof(int), 1, fileOUTPUT);
                    printf("%d ", item);
                }
                printf("\n");
            }
            printf("\n");
            free(mas_FIRST_str); free(mas_LAST_str); free(mas_indOTR); free(mas_indPOL);
            fclose(fileINPUT);
            fclose(fileOUTPUT);
            free(filename);
            free(outputname);
        }
        else{
            printf("Ошибка чтения файла .bin\n");
            fclose(fileINPUT);
            if (filename != NULL){free(filename);}
        }
}

int main(){
  //  recordBin();
    readingBin(); 
    return 0;
}