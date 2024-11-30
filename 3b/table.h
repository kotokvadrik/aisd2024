#ifndef TABLE_H
#define TABLE_H
#define MAX_SIZE 5

typedef struct Node {
    // номер версии элемента
    int release;
    // указатель на информацию
    int info;
    // указатель на следующий элемент
    struct Node* next;
} Node;

typedef struct KeySpace {
    // ключ элемента
    int key;
    // количество версий элемента
    int count_release;
    // указатель на информацию
    Node* node;
    // указатель на следующий элемент
    struct KeySpace* next;
} KeySpace;

typedef struct Hashtable {
    int msize;
    int csize;
    KeySpace** table;
} Hashtable;

#endif