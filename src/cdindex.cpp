/* 
  cdindex library.
  Copyright (C) 2017 Russell J. Funk <russellfunk@gmail.com>
   
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

#include <stdlib.h>
#include <stdbool.h>
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

   /* Build a list of "it" vertices that are "in_edges" of the focal vertex's
     "out_edges" as of timestamp t. Vertices in the list are unique. */

   size_t it_count = 0;
   vertex_id_t *it = (vertex_id_t *)malloc(sizeof(vertex_id_t));

   /* check for malloc problems */
   if (it==NULL) {
     raise_error(0);
   }

   /* define i for multiple loops */
   size_t i;

   /* add unique "in_edges" of focal vertex "out_edges" */
   for (i = 0; i < get_vertex_out_degree(graph, id); i++) {
     vertex_id_t out_edge_i = get_vertex_out_edge(graph, id, i);
     for (size_t j = 0; j < get_vertex_in_degree(graph, out_edge_i); j++) {
       vertex_id_t out_edge_i_in_edge_j = get_vertex_in_edge(graph, out_edge_i, j);
       if (get_vertex_timestamp(graph, out_edge_i_in_edge_j) > get_vertex_timestamp(graph, id) &&
           get_vertex_timestamp(graph, out_edge_i_in_edge_j) <= (get_vertex_timestamp(graph, id) + time_delta) &&
           !in_int_array(it, it_count, out_edge_i_in_edge_j)) {
         add_to_int_array(&it, it_count, out_edge_i_in_edge_j, true);
         it_count++;
       }
      }
     }

   /* add unique "in_edges" of focal vertex */
   for (i = 0; i < get_vertex_in_degree(graph, id); i++) {
     vertex_id_t in_edge_i = get_vertex_in_edge(graph, id, i);
     if (get_vertex_timestamp(graph, in_edge_i) > get_vertex_timestamp(graph, id) &&
         get_vertex_timestamp(graph, in_edge_i) <= (get_vertex_timestamp(graph, id) + time_delta) &&
         !in_int_array(it, it_count, in_edge_i)) {
       add_to_int_array(&it, it_count, in_edge_i, true);
       it_count++;
       }
     }

  /* compute the cd index */
  double sum_i = 0.0;
  for (i = 0; i < it_count; i++) {
    size_t f_it = vertex_has_out_edge(graph,it[i], id);
    size_t b_it = 0;
    for (size_t j = 0; j < get_vertex_out_degree(graph, it[i]); j++) {
      if (vertex_has_out_edge(graph, id, get_vertex_out_edge(graph, it[i], j))) {
        b_it = 1;
      }
    }
    sum_i += -2.0*f_it*b_it + f_it;
  }

  free(it);
  return sum_i/it_count;
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
   for (size_t i = 0; i < get_vertex_in_degree(graph, id); i++) {
     vertex_id_t in_edge_i = get_vertex_in_edge(graph, id, i);
     if (get_vertex_timestamp(graph, in_edge_i) <= (get_vertex_timestamp(graph, id) + time_delta)) {
       mt_count++;
       }
     }

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
