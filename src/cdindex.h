/* 
  fast-cdindex library.
  Copyright (C) 2017 Russell J. Funk <russellfunk@gmail.com>
  Copyright (C) 2023 Diomidis Spinellis <dds@aueb.gr>
   
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <set>
#include <new>

typedef long long int timestamp_t;

class GraphContainer;
typedef GraphContainer *Graph;

class VertexContainer;
typedef VertexContainer *Vertex;

typedef union {
	unsigned long long int id;
	Vertex v;
} vertex_id_t;

inline vertex_id_t
make_vertex_id(unsigned long long int id)
{
	vertex_id_t ret;
	ret.id = id;
	return ret;
}

inline vertex_id_t
make_vertex_id(Vertex v)
{
	vertex_id_t ret;
	ret.v = v;
	return ret;
}


class VertexContainer {

private:
  timestamp_t timestamp;
  std::set<Vertex> in_edges;
  std::set<Vertex> out_edges;

public:
  VertexContainer(timestamp_t t) : timestamp(t) {}

  const std::set<Vertex> &get_out_edges() { return out_edges; }
  const std::set<Vertex> &get_in_edges() { return in_edges; }

  size_t get_in_degree() { return in_edges.size(); }
  size_t get_out_degree() { return out_edges.size(); }

  timestamp_t get_timestamp() { return timestamp; }

  bool has_out_edge(Vertex out) {
      return out_edges.find(out) != out_edges.end();
  }

  friend void add_edge(vertex_id_t source_id, vertex_id_t target_id);

};

/**
 * \function add_edge
 * \brief Add a edge to a graph.
 *
 * \param graph The input graph.
 * \param source_id The source vertex id.
 * \param target_id The target vertex id.
 */
inline void add_edge(vertex_id_t source_id, vertex_id_t target_id) {

  source_id.v->out_edges.insert(target_id.v);
  target_id.v->in_edges.insert(source_id.v);
}

class GraphContainer {
private:
  std::set<Vertex> vs;

public:
  ~GraphContainer() {
    for (auto i : vs)
      delete i;
  }

  const std::set<Vertex> &get_vertices() { return vs; }

  size_t get_vcount() {
	  return vs.size();
  }

  size_t get_ecount() {
    size_t count = 0;
    for (auto i : vs)
      count += i->get_in_degree();
    return count;
  }

  /**
   * \function is_sane
   * \brief Run a few basic (not comprehensive) checks on graph data structure.
   *
   * \return Whether the graph is sane.
   */
  bool is_sane() {
    size_t in_edges = 0;
    size_t out_edges = 0;
    for (auto i : vs) {
      in_edges += i->get_in_degree();
      out_edges += i->get_out_degree();
    }
    return in_edges == out_edges;
  }

  /**
   * \function add_vertex
   * \brief Add a vertex to a graph.
   *
   * \param graph The input graph.
   * \param id The new vertex id.
   * \param timestamp The new vertex timestamp.
   */
  vertex_id_t add_vertex(timestamp_t timestamp) {
    Vertex v = new VertexContainer(timestamp);
    vs.insert(v);
    return make_vertex_id(v);
  }
};

/* function prototypes for utility.c */
void raise_error(int code);

struct vertex_s *get_vertex(Graph g, vertex_id_t id);

/* function prototypes for graph.cpp */
bool is_graph_sane(Graph graph); 
void add_vertex(Graph graph, vertex_id_t id, timestamp_t timestamp);
void free_graph(Graph graph);

vertex_id_t get_vertex_in_edge(Graph g, vertex_id_t vertex_id, size_t n);
bool vertex_has_out_edge(Graph g, vertex_id_t vertex_id, vertex_id_t out_vertex_id);
vertex_id_t make_vertex_id(unsigned long long int id);
vertex_id_t make_vertex_id(struct vertex_s *v);

/*
 * Vertex access functions.
 * These allow treating Vertex as an opaque data type
 */


Graph create_graph(void);
size_t get_graph_ecount(Graph g);
size_t get_graph_vcount(Graph g);

/* function prototypes for cdindex.c */
double cdindex(Graph graph, vertex_id_t id, timestamp_t time_delta);
double mcdindex(Graph graph, vertex_id_t id, timestamp_t time_delta);
size_t iindex(Graph graph, vertex_id_t id, timestamp_t time_delta);
