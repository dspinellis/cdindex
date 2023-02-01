fast-cdindex
=======

.. image:: https://readthedocs.org/projects/cdindex/badge/?version=latest
   :target: https://readthedocs.org/projects/cdindex/?badge=latest
   :alt: Documentation Status

fast-cdindex is a Python package, written in C,
for computing the CD index and other dynamic 
measures on evolving directed graphs.
It is based on the original `cdindex`_ package written by Russel Funk,
heavilly modified to improve its efficiency.
When a `Python program`_ is run on a graph with 116,568,934 vertices (publications) and
1,255,033,889 edges (citations) the original version

- inserts 109,045 vertices/s
- inserts 74,452 edges/s
- calculates 84 CD index values/s (based on the time for the first 3,000,000 vertices)
- requiring an estimated **16 days** to compute all CD index values using 47.9 GB of RAM.

This version

- inserts 90,998 vertices/s
- inserts 240,982 edges/s
- calculates 672 CD index values/s (based on the time for the first 3,000,000 vertices)
- requiring just **50 hours** (estimated) to compute all CD index values using 58 GB of RAM.

Furthermore, a `driver program`_ using this library,
but written entirely in C++ can utilize the processor's
8 cores to process the graph's 49.8 GB data structure *shared* among them,
and thus *actually* finish the task in a mere **9.5 hours**.

The library has been used to derive the `CD₅ index of works published in the period 1945-2016`_
from open data through `Alexandria3k`_.
More information about the process can be found in the following paper.

Diomidis Spinellis. Open Reproducible Systematic Publication Research. arXiv:2301.13312, January 2023. https://doi.org/10.48550/arXiv.2301.13312


.. _cdindex: https://github.com/russellfunk/cdindex
.. _Python program: https://github.com/dspinellis/alexandria3k/blob/main/examples/cdindex/cdindex-db.py
.. _driver program: https://github.com/dspinellis/alexandria3k/blob/main/examples/cdindex/cdindex-db.cpp
.. _Alexandria3k: https://github.com/dspinellis/alexandria3k
.. _CD₅ index of works published in the period 1945-2016: https://doi.org/10.5281/zenodo.7584373



Install
-------

Install the latest version of fast-cdindex from the downloaded
repository or from PyPi (when released)::

    $ pip install .
    $ pip install fast_cdindex

Simple example
--------------

Create a graph with some dummy data and compute the CD index::

    >>> from fast_cdindex import cdindex
    >>> import datetime

    >>> # dummy vertices for python module tests
    >>> pyvertices= [{"name": "0Z", "time": datetime.datetime(1992, 1, 1)},
                     {"name": "1Z", "time": datetime.datetime(1992, 1, 1)},
                     {"name": "2Z", "time": datetime.datetime(1993, 1, 1)},
                     {"name": "3Z", "time": datetime.datetime(1993, 1, 1)},
                     {"name": "4Z", "time": datetime.datetime(1995, 1, 1)},
                     {"name": "5Z", "time": datetime.datetime(1997, 1, 1)},
                     {"name": "6Z", "time": datetime.datetime(1998, 1, 1)},
                     {"name": "7Z", "time": datetime.datetime(1999, 1, 1)}, 
                     {"name": "8Z", "time": datetime.datetime(1999, 1, 1)},
                     {"name": "9Z", "time": datetime.datetime(1998, 1, 1)},
                     {"name": "10Z", "time": datetime.datetime(1997, 1, 1)}]

    >>> # dummy edges for python module tests
    >>> pyedges = [{"source": "4Z", "target": "2Z"},
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
                   {"source": "10Z", "target": "4Z"}]
 

    >>> # create graph
    >>> graph = cdindex.Graph()

    >>> # add vertices
    >>> for vertex in pyvertices:
          graph.add_vertex(vertex["name"], cdindex.timestamp_from_datetime(vertex["time"]))

    >>> # add edges
    >>> for edge in pyedges:
          graph.add_edge(edge["source"], edge["target"])

    >>> # prepare for running algorithms on the graph
    >>> graph.prepare_for_searching()

    >>> graph.cdindex("4Z", int(datetime.timedelta(days=1825).total_seconds()))

    >>> graph.mcdindex("4Z", int(datetime.timedelta(days=1825).total_seconds()))

Further information
-------

- **Website:** http://www.cdindex.info
- **Source:** https://github.com/dspinellis/fast-cdindex
- **Bug reports:** Open an issue in this repository

License
-------

Released under the GNU General Public License (GPL) (see `LICENSE`)::

   Copyright (C) 2017 Russell J. Funk <russellfunk@gmail.com>
   Copyright (C) 2023 Diomidis Spinellis <dds@aueb.gr>
