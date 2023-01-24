#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct Vertex;

typedef struct Graph {
    long long int vcount;
    struct Vertex *vs;
    long long int ecount;
} Graph;

#define CREATE_GRAPH(G) Graph G = {.vcount = 0, .ecount = 0}

/* function prototypes for utility.c */
void raise_error(int code);
bool in_int_array(long long int *array, long long int sizeof_array, long long int value);
void add_to_int_array(long long int **array, long long int sizeof_array, long long int value, bool add_memory);
struct Vertex *get_vertex(Graph *g, long long int id);


/* function prototypes for graph.cpp */
bool is_graph_sane(Graph *graph); 
void add_vertex(Graph *graph, long long int id, long long int timestamp);
void add_edge(Graph *graph, long long int source_id, long long int target_id);
void free_graph(Graph *graph);

long long int get_vertex_id(Graph *g, long long int id);
long long int get_vertex_in_degree(Graph *g, long long int id);
long long int get_vertex_out_degree(Graph *g, long long int id);
long long int get_vertex_timestamp(Graph *g, long long int id);
long long int get_vertex_out_edge(Graph *g, long long int vertex_id, long long int edge_id);
long long int get_vertex_in_edge(Graph *g, long long int vertex_id, long long int edge_id);
long long int * get_vertex_in_edges(Graph *g, long long int id);
long long int * get_vertex_out_edges(Graph *g, long long int id);


/* function prototypes for cdindex.c */
double cdindex(Graph *graph, long long int id, long long int time_delta);
double mcdindex(Graph *graph, long long int id, long long int time_delta);
long long int iindex(Graph *graph, long long int id, long long int time_delta);
#if defined(__cplusplus)
}
#endif
