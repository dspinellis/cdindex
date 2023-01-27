#!/usr/local/bin/python
# -*- coding: utf-8 -*-

"""tests.py: This script runs simple tests on the cdindex module."""

__author__ = "Russell J. Funk and Diomidis Spinellis"
__copyright__ = "Copyright (C) 2019, 2023"

# built in modules
import datetime

# custom modules
from fast_cdindex import cdindex, timestamp_from_datetime
import fast_cdindex._cdindex as _cdindex

# dummy vertices for c module tests
ctimes = (694224000,
         694224000,
         725846400,
         725846400,
         788918400,
         852098400,
         883612800,
         915148800,
         915148800,
         883612800,
         852076800)

# dummy edges for c module tests
cedges = ((4,2),
         (4,0),
         (4,1),
         (4,3),
         (5,2),
         (6,2),
         (6,4),
         (7,4),
         (8,4),
         (9,4),
         (9,1),
         (9,3),
         (10,4))

# dummy vertices for python module tests
pyvertices= [{"name": "0Z", "time": datetime.datetime(1992, 1, 1)},
             {"name": "1Z", "time": datetime.datetime(1992, 1, 1)},
             {"name": "2Z", "time": datetime.datetime(1993, 1, 1)},
             {"name": "3Z", "time": datetime.datetime(1993, 1, 1)},
             {"name": "4Z", "time": datetime.datetime(1995, 1, 1)},
             {"name": "5Z", "time": datetime.datetime(1997, 1, 1)},
             {"name": "6Z", "time": datetime.datetime(1998, 1, 1)},
             {"name": "7Z", "time": datetime.datetime(1999, 1, 1)},
             {"name": "8Z", "time": datetime.datetime(1999, 1, 1)},
             {"name": "9Z", "time": datetime.datetime(1998, 1, 1)},
             {"name": "AZ", "time": datetime.datetime(1997, 1, 1)}]

# dummy edges for python module tests
pyedges = [{"source": "4Z", "target": "2Z"},
           {"source": "4Z", "target": "0Z"},
           {"source": "4Z", "target": "1Z"},
           {"source": "4Z", "target": "3Z"},
           {"source": "5Z", "target": "2Z"},
           {"source": "6Z", "target": "2Z"},
           {"source": "6Z", "target": "4Z"},
           {"source": "7Z", "target": "4Z"},
           {"source": "8Z", "target": "4Z"},
           {"source": "9Z", "target": "4Z"},
           {"source": "9Z", "target": "1Z"},
           {"source": "9Z", "target": "3Z"},
           {"source": "AZ", "target": "4Z"}]

# test time
TEST_TIME = 157680000
TEST_TIME_PY = datetime.timedelta(days=1825)

# tests for the c extension
def c_tests():
  """Run tests for c extension."""

  # create graph
  graph = _cdindex.Graph()

  # add vertices
  v2i = dict()  # From internal vertex id to external integer id
  i2v = dict()  # From external integer id to internal vertex id
  for id, time in enumerate(ctimes):
    v = _cdindex.add_vertex(graph, time)
    i2v[id] = v
    v2i[v] = id

  # add edges
  for source, target in cedges:
    _cdindex.add_edge(i2v[source], i2v[target])

  # examine the graph
  print("Vertices in graph: %s" % (_cdindex.get_vcount(graph)))
  print("Edges in graph: %s" % (_cdindex.get_ecount(graph)))
  print("Graph sanity: %s" % (_cdindex._is_graph_sane(graph)))

  # examine properties of a vertices
  def canonicalize(list):
    """Convert a list of vertice identifies to sorted integers"""
    return sorted([v2i[v] for v in list])

  vertices = _cdindex.get_vertices(graph)
  vertices.sort(key=lambda v : v2i[v])
  for vertex in vertices:
    print("%s: %-5s | %s: %-15s %s: %-10s %s: %-10s %s at %s: %-20s %s at %s: %-20s %s: %-20s %s: %-20s"
        % ("vertex", v2i[vertex],
           "timestamp", _cdindex.get_vertex_timestamp(vertex),
           "in degree", _cdindex.get_vertex_in_degree(vertex),
           "out degree", _cdindex.get_vertex_out_degree(vertex),
           "cd index", TEST_TIME, _cdindex.cdindex(vertex, TEST_TIME),
           "mcd index", TEST_TIME, _cdindex.mcdindex(vertex, TEST_TIME),
           "in edges", canonicalize(_cdindex.get_vertex_in_edges(vertex)),
           "out edges", canonicalize(_cdindex.get_vertex_out_edges(vertex))),
          flush=True)

# tests for the python module
def py_tests():
  """Run tests for python module."""

  # create graph
  graph = cdindex.Graph()

  # add vertices
  for vertex in pyvertices:
    graph.add_vertex(vertex["name"], timestamp_from_datetime(vertex["time"]))

  # add edges
  for edge in pyedges:
    graph.add_edge(edge["source"], edge["target"])

  # examine the graph
  print("Vertices in graph: %s" % (graph.vcount()))
  print("Edges in graph: %s" % (graph.ecount()))
  print("Graph sanity: %s" % (graph._is_graph_sane()))

  # examine properties of a vertices
  for vertex in graph.vertices():
    print("%s: %-5s | %s: %-15s %s: %-10s %s: %-10s %s at %s: %-20s %s at %s: %-20s %s: %-35s %s: %-35s"
        % ("vertex", vertex,
           "timestamp", graph.timestamp(vertex),
           "in degree", graph.in_degree(vertex),
           "out degree", graph.out_degree(vertex),
           "cd index", TEST_TIME_PY, graph.cdindex(vertex, int(TEST_TIME_PY.total_seconds())),
           "mcd index", TEST_TIME_PY, graph.mcdindex(vertex, int(TEST_TIME_PY.total_seconds())),
           "in edges", sorted(graph.in_edges(vertex)),
           "out edges", sorted(graph.out_edges(vertex))), flush=True)

def main():

  # run c tests
  c_tests()

  # run python tests
  py_tests()

  # generate random graph
  g = cdindex.RandomGraph(generations=(2,3,4,5,6,7,7,9), edge_fraction=1)
  
  # print some information
  print(g.ecount())
  print(g.vcount())

if __name__ == "__main__":
  main()
