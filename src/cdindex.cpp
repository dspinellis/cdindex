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

#include "cdindex.h"

/**
 * \function cdindex
 * \brief Computes the CD Index.
 *
 * \param graph The input graph.
 * \param id The focal vertex id.
 * \param time_delta Time beyond stamp of focal vertex to consider in measure.
 *
 * \return The value of the CD index.
 */
double cdindex(Graph graph, vertex_id_t id, timestamp_t time_delta){

   /* Build a set of "it" vertices that are "in_edges" of the focal vertex's
     "out_edges" as of timestamp t. */

   std::set<Vertex> it;

   /* add unique "in_edges" of focal vertex "out_edges" */
   for (auto out_edge_i : id.v->get_out_edges())
     for (auto out_edge_i_in_edge_j : out_edge_i->get_in_edges())
       if (out_edge_i_in_edge_j->get_timestamp() > id.v->get_timestamp() &&
           out_edge_i_in_edge_j->get_timestamp() <= (id.v->get_timestamp() + time_delta))
	 it.insert(out_edge_i_in_edge_j);

   /* add unique "in_edges" of focal vertex */
   for (auto in_edge_i : id.v->get_in_edges())
     if (in_edge_i->get_timestamp() > id.v->get_timestamp() &&
         in_edge_i->get_timestamp() <= (id.v->get_timestamp() + time_delta))
       it.insert(in_edge_i);

  /* compute the cd index */
  double sum_i = 0.0;
  for (auto i : it) {
    int f_it = i->has_out_edge(id.v);
    int b_it = 0;
    for (auto j : i->get_out_edges())
      if (id.v->has_out_edge(j)) {
        b_it = 1;
	break;
      }
    sum_i += -2.0*f_it*b_it + f_it;
  }

  return sum_i/it.size();
}

/**
 * \function iindex
 * \brief Computes the I Index (i.e., the in degree of the focal vertex at time t).
 *
 * \param graph The input graph.
 * \param id The focal vertex id.
 * \param time_delta Time beyond stamp of focal vertex to consider in computing the measure.
 *
 * \return The value of the I index.
 */
size_t iindex(Graph graph, vertex_id_t id, timestamp_t time_delta){

   /* count mt vertices that are "in_edges" of the focal vertex as of timestamp t. */
   size_t mt_count = 0;
   for (auto in_edge_i : id.v->get_in_edges())
     if (in_edge_i->get_timestamp() <= (id.v->get_timestamp() + time_delta))
       mt_count++;

  return mt_count;
}

/**
 * \function mcdindex
 * \brief Computes the mCD Index.
 *
 * \param graph The input graph.
 * \param id The focal vertex id.
 * \param time_delta Time beyond stamp of focal vertex to consider in computing the measure.
 *
 * \return The value of the mCD index.
 */
double mcdindex(Graph graph, vertex_id_t id, timestamp_t time_delta){

  double cdindex_value = cdindex(graph, id, time_delta);
  size_t iindex_value = iindex(graph, id, time_delta);

  return cdindex_value * iindex_value;

}
