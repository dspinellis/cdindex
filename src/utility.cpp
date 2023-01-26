/* 
  fast-cdindex library.
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
#include <stdio.h>
#include <stdbool.h>
#include "cdindex.h"

/**
 * \function raise_error
 * \brief Raise an error.
 *
 * \param code The error code.
 */
void raise_error(int code) {
  const char *error[4];
  error[0] = "Problem (re)allocating memory";
  error[1] = "Vertex ids must be added sequentially from 0";
  error[2] = "One or more vertices are not in the graph";
  error[3] = "The edge being added is already in the graph";

  fprintf(stderr, "%s\n", error[code]);
  exit(EXIT_FAILURE);
}
