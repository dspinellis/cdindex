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

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <set>
#include <new>

#include "cdindex.h"


/*
 * Graph ADT access functions
 * These allow treating Graph as an opaque data type
 */
Graph
create_graph(void)
{
	Graph g = new GraphContainer();
	return g;
}



/**
 * \function free_graph
 * \brief Free memory taken by a graph.
 *
 * \param graph The input graph.
 */
void free_graph(Graph graph) {

  delete graph;
}
