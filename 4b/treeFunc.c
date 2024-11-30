#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "treeFunc.h"
#include "dialog.h"
#include <time.h>
#define BLACK 'b'
#define RED 'r'
#define LEFT_SON 1
#define RIGHT_SON 2

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

void eraseTree(Node** tree, Node* ELIST){
    if (*tree && *tree != ELIST){
        eraseTree(&((*tree)->left), ELIST);
        eraseTree(&((*tree)->right), ELIST);
        free(*tree);
    }
    *tree = NULL;
}

void Show(Node *ptr, int level, Node* ELIST){
    int i = level;
    if (ptr != NULL && ptr != ELIST){
        Show(ptr->right, level + 1, ELIST);
        while (i-- > 0){
            printf("			");
        }
        if(level != 0){printf("key: \"%d\" info: %d color: %c, parent key: %d\n\n", ptr->key, ptr->info, ptr->color, ptr -> parent -> key);}
        else{printf("key: \"%d\" info: %d color: %c\n\n", ptr->key, ptr->info, ptr->color);}
        Show(ptr->left, level + 1, ELIST);
    }
}

int Add(Node** tree, int key, int info, Node* ELIST){
    Node* par = NULL;
    Node* ptr = *tree;
    while (ptr != ELIST && ptr != NULL){
        if (ptr->key == key) {
            return 1;
        }
        par = ptr;
        ptr = (ptr->key < key) ? (ptr->right) : (ptr->left);
    }
 /*   if(*tree == NULL){
    	*tree = (Node*)calloc(1,sizeof(Node));
    	(*tree)->key = key;
    	(*tree)->info = info;
    	(*tree)->left = ELIST;
    	(*tree)->right = ELIST;
    	(*tree)->color = BLACK;
    	(*tree)->parent = NULL;  
    } */
    ptr = (Node *)calloc(1, sizeof(Node));
    ptr->key = key;
    ptr->info = info;
    ptr->left = ELIST;
    ptr->right = ELIST;
    ptr->color = 'r';
    ptr->parent = NULL;
    if (par){
    	ptr->parent = par;
        if (par->key > key){par->left = ptr;}
        else{par->right = ptr;}
    }
    if(!(ptr->parent)){ptr->color = 'b'; *tree = ptr; return 0;}
    if(ptr->parent->color == 'r'){add_balance(ptr, tree, ELIST);}
    return 0;
}

void Delete(Node** tree, int key, Node* ELIST){
    Node* founded_node = Find(*tree, key, ELIST);
    Node* parent = founded_node->parent;
    if(founded_node == NULL){return;}
    if(founded_node->left != ELIST && founded_node->right != ELIST){
        Node* min_node = FindMin(founded_node->right, ELIST);
        Node* min_node_parent = min_node->parent;
        int t = founded_node->key;
        int p = founded_node->info;
        founded_node->key = min_node->key;
        founded_node->info = min_node->info;
        min_node->key = t;
        min_node->info = p;
        del_balance(min_node, tree, ELIST);
        if(min_node_parent->left == min_node){min_node_parent->left = min_node -> right;}
        if(min_node_parent->right == min_node){min_node_parent->right = min_node -> right;}
        free(min_node);
        return;
    }
    if(parent != NULL){
        if(parent->left == founded_node){del_balance(founded_node, tree, ELIST); parent->left = (founded_node->left) ? founded_node->left: founded_node->right; free(founded_node); return;}
        if(parent->right == founded_node){del_balance(founded_node, tree, ELIST); parent->right = (founded_node->left) ? founded_node->left: founded_node->right; free(founded_node); return;}
    }
    if(parent == NULL){
        Node* max_node = FindMax(founded_node->left, ELIST);
        if (max_node == NULL){free(founded_node); *tree = NULL; return;}
        Node* max_node_parent = max_node->parent;
        founded_node->key = max_node->key;
        founded_node->info = max_node->info;
        if(max_node_parent->left == max_node){max_node_parent->left = max_node -> right;}
        if(max_node_parent->right == max_node){max_node_parent->right = max_node -> right;}
        del_balance(max_node, tree, ELIST);
        free(max_node);
        return;
    }
    del_balance(founded_node, tree, ELIST);
    free(founded_node);
}

void add_balance(Node* ptr, Node** tree, Node* ELIST){
    if(ptr == *tree){ptr->color = 'b'; return;}
    Node* father = ptr->parent;
    if(father->color == BLACK){return;}
    Node* grandpa = father->parent;
    Node* uncle = (father == grandpa->left) ? (grandpa->right) : grandpa->left;
    if(uncle->color == 'r'){
        father->color = 'b';
        uncle->color = 'b';
        grandpa->color = RED;
        add_balance(grandpa, tree, ELIST);
        return;
    }
    if(grandpa->left == father && ptr == father->right && uncle->color == BLACK){
        left_rotate(father, ELIST);
        ptr->color = BLACK;
        grandpa->color = RED;
        right_rotate(grandpa, ELIST);
        if(*tree == grandpa){*tree = grandpa->parent;}
    }
    else if(grandpa->right == father && ptr == father->left && uncle->color == BLACK){
        right_rotate(father, ELIST);
        ptr->color = BLACK;
        grandpa->color = RED;
        left_rotate(grandpa, ELIST);
        if(*tree == grandpa){*tree = grandpa->parent;}
    }
    else if(grandpa->right == father && ptr == father->right && uncle->color == BLACK){
    	father->color = BLACK;
        grandpa->color = RED;
        left_rotate(grandpa, ELIST);
        if(*tree == grandpa){*tree = grandpa->parent;}
    }
    else if(grandpa->left == father && ptr == father->left && uncle->color == BLACK){
    	father->color = BLACK;
    	grandpa->color = RED;
    	right_rotate(grandpa, ELIST);
    	if(*tree == grandpa){*tree = grandpa->parent;}
    }
    (*tree)->color = BLACK;
}

void del_balance(Node* ptr, Node** tree, Node* ELIST){
	printf("balancec\n");
    if(ptr->color == RED && ptr->left == ELIST && ptr->right == ELIST){
    	printf("aaaa\n");
        return;
    }
    if(ptr->color == BLACK && ((ptr->left != ELIST && ptr->right == ELIST) || (ptr->left == ELIST && ptr->right != ELIST))){
    	printf("bbbb\n");
        Node* son = (ptr->left != ELIST) ? (ptr->left) : ptr->right;
        if (son->left == ELIST && son->right == ELIST){
            son->color = BLACK;
        }
    }
    printf("ptr color is %c, ptr key is %d\n", ptr->color, ptr-> key);
    Node* brother, * left_son, * right_son; int indic_ptr, indic_brothers_son;
    while(ptr->color == BLACK && ptr != *tree){
    	printf("black cycle cc\n");
        brother = (ptr->parent->left == ptr) ? (ptr->parent->right) : (ptr->parent->left);
        printf("cc\n");
        indic_ptr = (ptr->parent->left == ptr) ? (LEFT_SON) : (RIGHT_SON);
        printf("cc\n");
        left_son = brother->left;
        printf("cc\n");
        right_son = brother->right;
        printf("cc\n");
        printf("brother %d color: %c\n", brother -> key, brother -> color);
        if(brother->color == RED){
        	printf("brother color red\n");
            brother->parent->color = RED;
            brother->color = BLACK;
            if(indic_ptr == LEFT_SON){left_rotate(brother->parent, ELIST); if(*tree == brother->left){*tree = brother -> left -> parent;}}
            else if(indic_ptr == RIGHT_SON){right_rotate(brother->parent, ELIST); if(*tree == brother->right){*tree = brother -> right -> parent;}}
        }
        brother = (ptr->parent->left == ptr) ? (ptr->parent->right) : (ptr->parent->left);
        left_son = brother->left;
        right_son = brother->right;
        indic_ptr = (ptr->parent->left == ptr) ? (LEFT_SON) : (RIGHT_SON);
        if(brother->color == BLACK){
        	printf("brother color black\n");
        	printf("left - %p, right - %p\n", left_son, right_son);
            if(left_son->color == RED || right_son->color == RED){ // 1.1
            	printf("hotya bi 1 red\n");
                if(left_son->color == RED && indic_ptr == LEFT_SON && right_son->color == BLACK || right_son->color == RED && indic_ptr == RIGHT_SON && left_son->color == BLACK){ // 1.1 b
					printf("1.1 b\n");
					printf("brother %d color: %c\n", brother -> key, brother -> color);
                    if(left_son->color == RED && indic_ptr == LEFT_SON && right_son->color == BLACK){
                        left_son->color = BLACK;
                        brother->color = RED;
                        right_rotate(brother, ELIST); if(*tree == brother){*tree = brother -> parent;}
                    }
                    else if(right_son->color == RED && indic_ptr == RIGHT_SON && left_son->color == BLACK){
                        right_son->color = BLACK;
                        brother->color = RED;
                        left_rotate(brother, ELIST); if(*tree == brother){*tree = brother -> parent;}
                    }
                }
                indic_ptr = (ptr->parent->left == ptr) ? (LEFT_SON) : (RIGHT_SON);
                brother = (ptr->parent->left == ptr) ? (ptr->parent->right) : (ptr->parent->left);
                left_son = brother->left;
                right_son = brother->right;
                if(left_son->color == RED && indic_ptr == RIGHT_SON || right_son->color == RED && indic_ptr == LEFT_SON){ // 1.1 a
                	printf("1.1 a\n");
                	printf("brother %d color: %c\n", brother -> key, brother -> color);
                    brother->color = brother->parent->color;
                    brother->parent->color = BLACK;
                    if(left_son->color == RED && indic_ptr == RIGHT_SON){
                        left_son->color = BLACK;
                        right_rotate(brother->parent, ELIST); if(*tree == brother->right){*tree = brother -> right -> parent;}
                    }
                    else{
                        right_son->color = BLACK;
                        left_rotate(brother->parent, ELIST); if(*tree == brother->left){*tree = brother -> left -> parent;}
                    }
                    printf("brothers father %d\n", brother -> parent -> key);
                    
                    return;
                }
            }
            if(left_son->color == BLACK && right_son->color == BLACK){
                printf("1.2\n");
                brother->color = RED;
                printf("brother parent is %d\n", brother -> parent -> key);
                if(brother->parent->color == BLACK) {del_balance(brother->parent, tree, ELIST); return;}
                if(brother->parent->color == RED){brother->parent->color = BLACK; return;}
            }
        }
    }
}

void right_rotate(Node* ptr, Node* ELIST){
    Node* left_son = ptr->left;
    if(left_son -> right != ELIST && left_son -> right != NULL){
    	left_son -> right -> parent = ptr;
    }
    ptr->left = left_son->right;
    left_son->right = ptr;    
    left_son->parent = ptr->parent;
    ptr->parent = left_son;
    if(left_son->parent && left_son->parent->left == ptr){left_son->parent->left = left_son;}
    else if(left_son->parent && left_son->parent->right == ptr){left_son->parent->right = left_son;}
}

void left_rotate(Node* ptr, Node* ELIST){
    Node* right_son = ptr->right;
    if(right_son -> left != ELIST && right_son -> left != NULL){
        right_son -> left -> parent = ptr;
    }
    ptr->right = right_son->left;
    right_son->left = ptr;
    right_son->parent = ptr->parent;
    ptr->parent = right_son;
    if(right_son->parent && right_son->parent->left == ptr){right_son->parent->left = right_son;}
    else if(right_son->parent && right_son->parent->right == ptr){right_son->parent->right = right_son;}
}

void TreeTraversal(Node* tree, Node* ELIST){
    if (tree != ELIST && tree != NULL){
        TreeTraversal(tree->left, ELIST);
        TreeTraversal(tree->right, ELIST);
        printf("%d ", tree->key);
    }
}

Node* FindMin(Node* tree, Node* ELIST){
    if (!tree || tree == ELIST){return NULL;}
    while (tree->left != ELIST) tree = tree->left;
    return tree;
}

Node* FindMax(Node* tree, Node* ELIST){
    if(!tree || tree == ELIST){return NULL;}
    while(tree -> right != ELIST) tree = tree->right;
    return tree;
}

Node* Find(Node* tree, int key, Node* ELIST){
    while (tree != ELIST && tree != NULL){
        if (key == tree->key) return tree;
        tree = (key < tree->key) ? tree->left : tree->right;
    }
    return NULL;
}

void makedot(Node* tree, Node* ELIST){
    printf("Enter the name of dotfile (+.dot): ");
    char* filename = myreadline();
    FILE* f = fopen(filename, "w+t");
    fprintf(f, "digraph G{");
    output_in_dot(tree,f, ELIST);
    fprintf(f,"\n}");
    free(filename);
    fclose(f);
}

void output_in_dot(Node* tree, FILE* f, Node* ELIST){
    if (tree != NULL && tree != ELIST){
        if(tree->left != ELIST){
            fprintf(f, "\n\t%d->%d;", tree->key, tree->left->key);
        }
        if(tree->right != ELIST){
            fprintf(f, "\n\t%d->%d;", tree->key, tree->right->key);
        }
        output_in_dot(tree->left, f, ELIST);
        output_in_dot(tree->right, f, ELIST);
    }
}

void Output(Node* tree, FILE* f, Node* ELIST){// в диалоге открыть и закрыть
    if (tree != NULL && tree != ELIST){
        fprintf(f, "%d %d\n", tree->key, tree->info);
        Output(tree->left,f, ELIST);
        Output(tree->right,f, ELIST);
    }
}

void Import(Node**tree, FILE* f, Node* ELIST){ // в диалоге открыть и закрыть
    eraseTree(tree, ELIST);
    char* line = myfilereadline(f); char* word; int key; int info;
    int item;
    while(line != NULL){
        word = strtok(line, " ");
        key = atoi(word);
        word = strtok(NULL, " ");
        info = atoi(word);
        item = Add(tree,key,info,ELIST);
        free(line);
        line = myfilereadline(f);
    }
}

void output_in_dop(Node* tree, FILE* f, Node* ELIST){
    if (tree != NULL && tree != ELIST){
        fprintf(f, "number: %d; line: %d;\n", tree->key, tree->info);
        output_in_dop(tree->left, f, ELIST);
        output_in_dop(tree->right, f, ELIST);
    }
}

void Timing(Node** tree, int n, Node* ELIST){
    eraseTree(tree, ELIST);
    srand(time(NULL));
    int key;
    int info;
    clock_t start, finish, summary;
    Node* item;
    int random_key_int1, random_key_int2, random_key_int3;
    int mas[3];
    mas[0] = n-1;
    mas[1] = n-2;
    mas[2] = n-3;
    for(int i = 0; i < n; i++){
        info = rand()%50;
        key = rand()%100000;
        while(Add(tree, key, info, ELIST) == 0){
            key = rand()%100000;
        }
        if (i == mas[0]){
            random_key_int1 = key;
        }
        if (i == mas[1]){
            random_key_int2 = key;
        }
        if (i == mas[2]){
            random_key_int3 = key;
        }
    }
    int masint[3];
    masint[0] = random_key_int1; masint[1] = random_key_int2; masint[2] = random_key_int3;
    printf("Tree was generated\n");
    for(int k = 0; k < 3; k++){
        start = clock();
        item = Find(*tree, masint[k], ELIST);
        finish = clock();
        printf("Время поиска ключа (на входе %d элементов) - %lf - finish, %lf - start\n", n, (double)(finish), (double)(start));
    }
    while(Find(*tree, key, ELIST) != NULL){
        key = rand()%100000;
    }
    int tup_per;
    start = clock();
    tup_per = Add(tree, key, info, ELIST);
    finish = clock();
    printf("Время вставки ключа (на входе %d элементов) - %lf - finish, %lf - start\n", n, (double)(finish), (double)(start));
    
    key = masint[0];
    start = clock();
    Delete(tree, key, ELIST);
    finish = clock();
    printf("Время удаления ключа (на входе %d элементов) - %lf - finish, %lf - start\n", n, (double)(finish), (double)(start));
    
    key = masint[1];
    start = clock();
    Delete(tree, key, ELIST);
    finish = clock();
    printf("Время удаления ключа (на входе %d элементов) - %lf - finish, %lf - start\n", n, (double)(finish), (double)(start));
    
    key = masint[2];
    start = clock();
    Delete(tree, key, ELIST);
    finish = clock();
    printf("Время удаления ключа (на входе %d элементов) - %lf - finish, %lf - start\n", n, (double)(finish), (double)(start));
}
