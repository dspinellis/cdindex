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

#include <algorithm>
#include <cstddef>
#include <new>
#include <vector>


typedef long long int timestamp_t;
class Vertex;

/*
 * A data type that allows accessing graph vertices either
 * through their Python-visible integer id (id), or through
 * the internally-used Vertex pointer (v).
 */
typedef union {
	unsigned long long int id;
	Vertex *v;
} vertex_id_t;

inline vertex_id_t
make_vertex_id(unsigned long long int id)
{
	vertex_id_t ret;
	ret.id = id;
	return ret;
}

inline vertex_id_t
make_vertex_id(Vertex *v)
{
	vertex_id_t ret;
	ret.v = v;
	return ret;
}

class Vertex {

private:
  timestamp_t timestamp;
  std::vector<Vertex *> in_edges;
  std::vector<Vertex *> out_edges;

public:
  Vertex(timestamp_t t) : timestamp(t) {}

  const std::vector<Vertex *> &get_out_edges() const { return out_edges; }
  const std::vector<Vertex *> &get_in_edges() const { return in_edges; }

  size_t get_in_degree() const { return in_edges.size(); }
  size_t get_out_degree() const { return out_edges.size(); }

  timestamp_t get_timestamp() const { return timestamp; }

  // Reduce used memory from capacity to what is actuall required
  void shrink_to_fit() {
    out_edges.shrink_to_fit();
    in_edges.shrink_to_fit();
  }

  // Sort out_edges to allow binary search on them
  void sort_out_edges() {
      std::sort(out_edges.begin(), out_edges.end());
  }

  bool has_out_edge(Vertex *out) const {
      return std::binary_search(out_edges.begin(), out_edges.end(),  out);
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
 *
 * Adges must be added only once.
 */
inline void add_edge(vertex_id_t source_id, vertex_id_t target_id) {

  source_id.v->out_edges.push_back(target_id.v);
  target_id.v->in_edges.push_back(source_id.v);
}

class Graph {
private:
  std::vector<Vertex *> vs;

public:
  ~Graph() {
    for (auto i : vs)
      delete i;
  }

  const std::vector<Vertex *> &get_vertices() { return vs; }

  size_t get_vcount() {
	  return vs.size();
  }

  size_t get_ecount() const {
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
  bool is_sane() const {

    size_t in_edges = 0;
    size_t out_edges = 0;
    for (auto i : vs) {
      in_edges += i->get_in_degree();
      out_edges += i->get_out_degree();
    }

    size_t found = 0;
    for (auto i : vs)
      for (auto j : i->get_out_edges())
	found += i->has_out_edge(j);

    return in_edges == out_edges && found == out_edges;
  }

  /**
   * \function prepare_for_searching
   * \brief Sort the out edges so that has_out_edge can use binary search
   */
  void prepare_for_searching() {
    for (auto i : vs) {
      // Improve locality of reference
      i->shrink_to_fit();
      i->sort_out_edges();
    }
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
    Vertex *v = new Vertex(timestamp);
    vs.push_back(v);
    return make_vertex_id(v);
  }
};

/* function prototypes for cdindex.c */
double cdindex(vertex_id_t id, timestamp_t time_delta);
double mcdindex(vertex_id_t id, timestamp_t time_delta);
size_t iindex(vertex_id_t id, timestamp_t time_delta);
