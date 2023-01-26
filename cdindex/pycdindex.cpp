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

#include <cassert>
#include <Python.h>
#include "cdindex.h"

extern "C" {

#if PY_MAJOR_VERSION >= 3
#define PY3K
#endif

/* Destructor function for Graph */
static void del_Graph(PyObject *obj) {
  delete (Graph)PyCapsule_GetPointer(obj,"Graph");
}

/* Graph utility functions */
static Graph PyGraph_AsGraph(PyObject *obj) {
  return (Graph)PyCapsule_GetPointer(obj, "Graph");
}
static PyObject *PyGraph_FromGraph(Graph g, int must_free) {
  return PyCapsule_New(g, "Graph", must_free ? del_Graph : NULL);
}

/*******************************************************************************
 * Create a new Graph object                                                   *
 ******************************************************************************/
static PyObject *py_Graph(PyObject *self, PyObject *args) {
  Graph g = new GraphContainer();
  return PyGraph_FromGraph(g, 1);
}

/*******************************************************************************
 * Check graph sanity                                                          *
 ******************************************************************************/
static PyObject *py_is_graph_sane(PyObject *self, PyObject *args) {
  Graph g;
  PyObject *py_g;
  vertex_id_t vid;

  // Ensure that the id can hold the pointer value
  assert(sizeof(vid.id) >= sizeof(vid.v));

  if (!PyArg_ParseTuple(args,"O",&py_g))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  return Py_BuildValue("O", g->is_sane() ? Py_True : Py_False);
}

/*******************************************************************************
 * Add a vertex to the graph                                                   *
 ******************************************************************************/
static PyObject *py_add_vertex(PyObject *self, PyObject *args) {
  timestamp_t TIMESTAMP;
  Graph g;
  PyObject *py_g;

  if (!PyArg_ParseTuple(args,"OL",&py_g, &TIMESTAMP))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  vertex_id_t ID = g->add_vertex(TIMESTAMP);

  return Py_BuildValue("L", ID.id);
}

/*******************************************************************************
 * Add an edge to the graph                                                    *
 ******************************************************************************/
static PyObject *py_add_edge(PyObject *self, PyObject *args) {
  vertex_id_t SOURCE_ID, TARGET_ID;
  Graph g;
  PyObject *py_g;

  if (!PyArg_ParseTuple(args,"OLL",&py_g, &SOURCE_ID, &TARGET_ID))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  add_edge(SOURCE_ID, TARGET_ID);

  return Py_BuildValue("");
}

/*******************************************************************************
 * Get a count of vertices in the graph                                        *
 ******************************************************************************/
static PyObject *py_get_vcount(PyObject *self, PyObject *args) {

  Graph g;
  PyObject *py_g;

  if (!PyArg_ParseTuple(args,"O",&py_g))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  return Py_BuildValue("L", g->get_vcount());
}

/*******************************************************************************
 * Get list of vertices in the graph                                           *
 ******************************************************************************/
static PyObject *py_get_vertices(PyObject *self, PyObject *args) {

  Graph g;
  PyObject *py_g, *id, *result;

  if (!PyArg_ParseTuple(args,"O",&py_g))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  PyObject *vs_list = PyList_New(g->get_vcount());

  size_t i = 0;
  for (auto v : g->get_vertices()) {
    id = Py_BuildValue("L", make_vertex_id(v).id);
    PyList_SetItem(vs_list, i, id);
    i++;
  }

  result = Py_BuildValue("O", vs_list);

  // clean up 
  Py_DECREF(vs_list);

  return result;
}

/*******************************************************************************
 * Get a count of edges in the graph                                           *
 ******************************************************************************/
static PyObject *py_get_ecount(PyObject *self, PyObject *args) {

  Graph g;
  PyObject *py_g;

  if (!PyArg_ParseTuple(args,"O",&py_g))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  return Py_BuildValue("L", g->get_ecount());
}

/*******************************************************************************
 * Get a vertex timestamp                                                      *
 ******************************************************************************/
static PyObject *py_get_vertex_timestamp(PyObject *self, PyObject *args) {
  vertex_id_t ID;

  Graph g;
  PyObject *py_g;

  if (!PyArg_ParseTuple(args,"OL",&py_g, &ID))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  return Py_BuildValue("L", ID.v->get_timestamp());
}

/*******************************************************************************
 * Get a vertex in degree                                                      *
 ******************************************************************************/
static PyObject *py_get_vertex_in_degree(PyObject *self, PyObject *args) {
  vertex_id_t ID;

  Graph g;
  PyObject *py_g;

  if (!PyArg_ParseTuple(args,"OL",&py_g, &ID))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  return Py_BuildValue("L", ID.v->get_in_degree());
}

/*******************************************************************************
 * Get the in edges of a vertex                                                *
 ******************************************************************************/
static PyObject *py_get_vertex_in_edges(PyObject *self, PyObject *args) {

  vertex_id_t ID;
  Graph g;
  PyObject *py_g, *source_id, *result;

  if (!PyArg_ParseTuple(args,"OL",&py_g, &ID))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  PyObject *vs_list = PyList_New(ID.v->get_in_degree());

  size_t i = 0;
  for (auto v : ID.v->get_in_edges()) {
    source_id = Py_BuildValue("L", make_vertex_id(v).id);
    PyList_SetItem(vs_list, i, source_id);
    i++;
  }

  result = Py_BuildValue("O", vs_list);

  // clean up 
  Py_DECREF(vs_list);

  return result;
}

/*******************************************************************************
 * Get a vertex out degree                                                     *
 ******************************************************************************/
static PyObject *py_get_vertex_out_degree(PyObject *self, PyObject *args) {
  vertex_id_t ID;

  Graph g;
  PyObject *py_g;

  if (!PyArg_ParseTuple(args,"OL",&py_g, &ID))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  return Py_BuildValue("L", ID.v->get_out_degree());
}

/*******************************************************************************
 * Get the out edges of a vertex                                                *
 ******************************************************************************/
static PyObject *py_get_vertex_out_edges(PyObject *self, PyObject *args) {

  vertex_id_t ID;
  Graph g;
  PyObject *py_g, *target_id, *result;

  if (!PyArg_ParseTuple(args,"OL",&py_g, &ID))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  PyObject *vs_list = PyList_New(ID.v->get_out_degree());

  size_t i = 0;
  for (auto v : ID.v->get_out_edges()) {
    target_id = Py_BuildValue("L", make_vertex_id(v).id);
    PyList_SetItem(vs_list, i, target_id);
    i++;
  }

  result = Py_BuildValue("O", vs_list);

  // clean up 
  Py_DECREF(vs_list);

  return result;
}

/*******************************************************************************
 * Compute the CD index                                                        *
 ******************************************************************************/
static PyObject *py_cdindex(PyObject *self, PyObject *args) {
  vertex_id_t ID;
  timestamp_t TIMESTAMP;

  double result;
  Graph g;
  PyObject *py_g;

  if (!PyArg_ParseTuple(args,"OLL",&py_g, &ID, &TIMESTAMP))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  result = cdindex(g, ID, TIMESTAMP);
  
  return Py_BuildValue("d", result);
}

/*******************************************************************************
 * Compute the mCD index                                                       *
 ******************************************************************************/
static PyObject *py_mcdindex(PyObject *self, PyObject *args) {
  vertex_id_t ID;
  timestamp_t TIMESTAMP;
  double result;
  Graph g;
  PyObject *py_g;

  if (!PyArg_ParseTuple(args,"OLL",&py_g, &ID, &TIMESTAMP))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  result = mcdindex(g, ID, TIMESTAMP);
  
  return Py_BuildValue("d", result);
}

/*******************************************************************************
 * Compute the I index                                                       *
 ******************************************************************************/
static PyObject *py_iindex(PyObject *self, PyObject *args) {
  vertex_id_t ID;
  timestamp_t TIMESTAMP;
  double result;
  Graph g;
  PyObject *py_g;

  if (!PyArg_ParseTuple(args,"OLL",&py_g, &ID, &TIMESTAMP))
    return NULL;
  if (!(g = PyGraph_AsGraph(py_g)))
    return NULL;

  result = iindex(g, ID, TIMESTAMP);
  
  return Py_BuildValue("d", result);
}


/*******************************************************************************
 * Module method table                                                         *
 ******************************************************************************/
static PyMethodDef CDIndexMethods[] = {
  {"Graph",  py_Graph, METH_VARARGS, "Make a graph"},
  {"_is_graph_sane", py_is_graph_sane, METH_VARARGS, "Test graph sanity"},
  {"add_vertex", py_add_vertex, METH_VARARGS, "Add a vertex to a graph"},
  {"add_edge", py_add_edge, METH_VARARGS, "Add an edge to a graph"},
  {"get_vertices", py_get_vertices, METH_VARARGS, "Get a list of vertices in the graph"},
  {"get_vcount", py_get_vcount, METH_VARARGS, "Get the number of vertices in the graph"},
  {"get_ecount", py_get_ecount, METH_VARARGS, "Get the number of edges in the graph"},
  {"get_vertex_timestamp", py_get_vertex_timestamp, METH_VARARGS, "Get the timestamp of a vertex"},
  {"get_vertex_in_degree", py_get_vertex_in_degree, METH_VARARGS, "Get the in degree of a vertex"},
  {"get_vertex_in_edges", py_get_vertex_in_edges, METH_VARARGS, "Get the in edges of a vertex"},
  {"get_vertex_out_degree", py_get_vertex_out_degree, METH_VARARGS, "Get the out degree of a vertex"},
  {"get_vertex_out_edges", py_get_vertex_out_edges, METH_VARARGS, "Get the out edges of a vertex"},
  {"cdindex", py_cdindex, METH_VARARGS, "Compute the CD index"},
  {"mcdindex", py_mcdindex, METH_VARARGS, "Compute the mCD index"},
  {"iindex", py_iindex, METH_VARARGS, "Compute the I index"},
  { NULL, NULL, 0, NULL}
};

/*******************************************************************************
 * Module initialization function                                              *
 ******************************************************************************/


#ifdef PY3K
static struct PyModuleDef _cdindex =
{
    PyModuleDef_HEAD_INIT,
    "_cdindex", 
    "",         
    -1,         
    CDIndexMethods
};

PyMODINIT_FUNC PyInit__cdindex(void)
{
    return PyModule_Create(&_cdindex);
}
#else
PyMODINIT_FUNC
init_cdindex(void) {
    (void) Py_InitModule("_cdindex", CDIndexMethods);
}
#endif

} // extern "C"
