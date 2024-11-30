#ifndef GRAPH_H
#define GRAPH_H
#define MAX_VERTEX 100

typedef struct Edge{
    Vertex* vertex_to;
    int port_access[MAX_VERTEX];
    int delay;
    struct Edge* next;
} Edge;

typedef struct Vertex {
    char* name;
    int port;
    Edge** edge_list; 
    struct Vertex* next; 
} Vertex;

#endif
