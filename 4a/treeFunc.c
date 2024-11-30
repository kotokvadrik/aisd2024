#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "treeFunc.h"
#include "dialog.h"
#include <time.h>

char* myfilereadline(FILE* file){
    char *ptr = (char*)malloc(1);
    char buf[81];
    int n, len = 0;
    *ptr = '\0';
    do {
        n = fscanf(file,"%80[^\n]",buf);
        if (n < 0){
            free(ptr);
            ptr = NULL;
            continue;
        }
        if (n == 0){fscanf(file,"%*c");}
        else {
            len += strlen(buf);
            ptr = (char*) realloc(ptr,len + 1);
            strcat(ptr,buf);
        }
    } while(n > 0);
    return ptr;
}

void eraseTree(Node** tree){
    if (*tree){
        eraseTree(&((*tree)->left));
        eraseTree(&((*tree)->right));
        free((*tree) -> key);
        free(*tree);
    }
    *tree = NULL;
}

void Show(Node *ptr, int level){
    int i = level;
    if (ptr){
        Show(ptr->right, level + 1);
        while (i-- > 0){
            printf("			");
        }
        printf("key: \"%s\" info: %d\n\n", ptr->key, ptr->info);
        Show(ptr->left, level + 1);
    }
}

Node* FindMin(Node* tree){
    if (!tree){return NULL;}
    while (tree->left) tree = tree->left;
    return tree;
}

Node* Find(Node* tree, char* key, int code){ // code == 1 - item, code == 2 - parent
    Node* par = NULL;
    while (tree){
        if (strcmp(key,tree->key) == 0 && code == 1) return tree;
        if (strcmp(key,tree->key) == 0 && code == 2) return par;
        par = tree;
        tree = strcmp(key,tree->key) < 0 ? tree->left : tree->right;
    }
    return NULL;
}

Node* Add(Node** tree, char* key, int info){
    Node* par = NULL;
    while (*tree){
        if (strcmp((*tree)->key, key) == 0) {
            Node* res = calloc(1, sizeof(Node));
            res->key = calloc(strlen(key)+1,sizeof(char));
            strcpy(res->key, key);
            res->info = (*tree) -> info;
            res->left = NULL;
            res->right = NULL;
            (*tree) -> info = info;
            return res;
        }
        par = *tree;
        tree = (strcmp((*tree)->key, key) < 0) ? &((*tree)->right) : &((*tree)->left);
    }
    *tree = (Node *)calloc(1, sizeof(Node));
    (*tree)->key = calloc(strlen(key)+1,sizeof(char));
    strcpy((*tree)->key, key);
    (*tree)->info = info;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    if (par){
        if (strcmp(par->key, key) > 0){par->left = *tree;}
        else{par->right = *tree;}
    }
    return NULL;
}

void Delete(Node** tree, char* key){
    Node* parent = Find(*tree, key, 2);
    Node* founded_node = Find(*tree, key, 1);
    if(founded_node == NULL){return;}
    if(founded_node->left != NULL && founded_node->right != NULL){
        Node* min_node = FindMin(founded_node->right);
        Node* min_node_parent = Find(founded_node, min_node->key, 2);
        founded_node->key = realloc(founded_node->key, (strlen(min_node->key)+1) * sizeof(char));
        strcpy(founded_node->key, min_node->key);
        founded_node->info = min_node->info;
        free(min_node->key);
        if(min_node_parent->left == min_node){min_node_parent->left = min_node -> right;}
        if(min_node_parent->right == min_node){min_node_parent->right = min_node -> right;}
        free(min_node);
        return;
    }
    free(founded_node->key);
    if(parent != NULL){
        if(parent->left == founded_node){parent->left = (founded_node->left) ? founded_node->left: founded_node->right;}
        if(parent->right == founded_node){parent->right = (founded_node->left) ? founded_node->left: founded_node->right;}
    }
    if(parent == NULL){
        if(founded_node->left != NULL){*tree = founded_node->left;}
        else if(founded_node->right != NULL){*tree = founded_node->right;}
        else{*tree = NULL;}
    }
    free(founded_node);
    return;
}

void TreeTraversal(Node* tree){
    if (tree != NULL){
        TreeTraversal(tree->left);
        TreeTraversal(tree->right);
        printf("%s ", tree->key);
    }
}

void output_in_dot(Node* tree, FILE* f){
    if (tree != NULL){
        if(tree->left != NULL){
            fprintf(f, "\n\t%s->%s;", tree->key, tree->left->key);
        }
        if(tree->right != NULL){
            fprintf(f, "\n\t%s->%s;", tree->key, tree->right->key);
        }
        output_in_dot(tree->left, f);
        output_in_dot(tree->right, f);
    }
}

void output_in_dop(Node* tree, FILE* f){
    if (tree != NULL){
        fprintf(f, "word: %s; number: %d;\n", tree->key, tree->info);
        output_in_dop(tree->left, f);
        output_in_dop(tree->right, f);
    }
}

void makedot(Node* tree){
    printf("Enter the name of dotfile (+.dot): ");
    char* filename = myreadline();
    FILE* f = fopen(filename, "w+t");
    fprintf(f, "digraph G{");
    output_in_dot(tree,f);
    fprintf(f,"\n}");
    fclose(f);
}

void Output(Node* tree, FILE* f){// в диалоге открыть и закрыть
    if (tree != NULL){
        fprintf(f, "%s %d\n", tree->key, tree->info);
        Output(tree->left,f);
        Output(tree->right,f);
    }
}

void Import(Node**tree, FILE* f){ // в диалоге открыть и закрыть
    eraseTree(tree);
    char* line = myfilereadline(f); char* word; char* key; int info;
    Node* item;
    while(line != NULL){
        word = strtok(line, " ");
        key = calloc(strlen(word)+1, sizeof(char));
        strcpy(key, word);
        word = strtok(NULL, " ");
        info = atoi(word);
        item = Add(tree,key,info);
        free(key);
        free(line);
        line = myfilereadline(f);
    }
}
/*
    Генерирование рандомных ключей длины 5-6-7 и вставка в дерево, 
    по запросу пользователя количество. 
    Информация - рандомное число. Исследование поиска. 
    (Рандомный выбор ключа в процессе вставки, счётчик от n - количества)
*/
void Timing(Node** tree, int n){
    eraseTree(tree);
    srand(time(NULL));
    char* key;
    int info;
    char letter;
    clock_t start, finish, summary;
    int random_key = rand()%n; Node* item;
    char* random_key_str1, * random_key_str2, * random_key_str3;
    int mas[3];
    mas[0] = n-1;
    random_key = rand()%n;
    mas[1] = n-2;
    random_key = rand()%n;
    mas[2] = n-3;
    for(int i = 0; i < n; i++){
        info = rand()%2000;
        key = calloc(9, sizeof(char));
        do{
        	for (int j = 0; j < 8; j++){
	           letter = rand()%26+97;
	           key[j] = letter;
        	}
        } while(Find(*tree, key, 1) != NULL);
        Add(tree, key, info);
        if (i == mas[0]){
            random_key_str1 = calloc(strlen(key)+1, sizeof(char));
            strcpy(random_key_str1, key);
            printf("str - %s\n", random_key_str1);
        }
        if (i == mas[1]){
            random_key_str2 = calloc(strlen(key)+1, sizeof(char));
            strcpy(random_key_str2, key);
            printf("str - %s\n", random_key_str2);
        }
        if (i == mas[2]){
            random_key_str3 = calloc(strlen(key)+1, sizeof(char));
            strcpy(random_key_str3, key);
            printf("str - %s\n", random_key_str3);
        }
        free(key);
    }
    char* masstr[3];
    masstr[0] = random_key_str1; masstr[1] = random_key_str2; masstr[2] = random_key_str3;
    printf("Tree was generated\n");
    for(int k = 0; k < 3; k++){
        start = clock();
        item = Find(*tree, masstr[k], 1);
        printf("str - %s, info - %d\n", item -> key, item -> info);
        finish = clock();
        printf("Время поиска ключа (на входе %d элементов) - %lf - finish, %lf - start\n", n, (double)(finish), (double)(start));
        free(masstr[k]);
    }
}
