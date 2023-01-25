#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct vertex_s;
struct graph_s;

typedef union {
	unsigned long long int id;
	struct vertex_s *v;
} vertex_id_t;

typedef long long int timestamp_t;

typedef struct graph_s *Graph;

/* function prototypes for utility.c */
void raise_error(int code);
bool in_int_array(vertex_id_t *array, size_t sizeof_array, vertex_id_t value);
void add_to_int_array(vertex_id_t **array, size_t sizeof_array, vertex_id_t value, bool add_memory);
struct vertex_s *get_vertex(Graph g, vertex_id_t id);


/* function prototypes for graph.cpp */
bool is_graph_sane(Graph graph); 
void add_vertex(Graph graph, vertex_id_t id, timestamp_t timestamp);
void add_edge(Graph graph, vertex_id_t source_id, vertex_id_t target_id);
void free_graph(Graph graph);

size_t get_vertex_in_degree(Graph g, vertex_id_t id);
size_t get_vertex_out_degree(Graph g, vertex_id_t id);
timestamp_t get_vertex_timestamp(Graph g, vertex_id_t id);
vertex_id_t get_vertex_out_edge(Graph g, vertex_id_t vertex_id, size_t n);
vertex_id_t get_vertex_in_edge(Graph g, vertex_id_t vertex_id, size_t n);
bool vertex_has_out_edge(Graph g, vertex_id_t vertex_id, vertex_id_t out_vertex_id);
vertex_id_t make_vertex_id(unsigned long long int id);

Graph create_graph(void);
Graph create_graph(void);
size_t get_graph_ecount(Graph g);

/* function prototypes for cdindex.c */
double cdindex(Graph graph, vertex_id_t id, timestamp_t time_delta);
double mcdindex(Graph graph, vertex_id_t id, timestamp_t time_delta);
size_t iindex(Graph graph, vertex_id_t id, timestamp_t time_delta);
#if defined(__cplusplus)
}
#endif
