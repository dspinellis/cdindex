#!/usr/local/bin/python
# -*- coding: utf-8 -*-

"""setup.py: This script installs the cdindex python module."""

__author__ = "Russell J. Funk"
__copyright__ = "Copyright (C) 2019"

# built in modules
from setuptools import setup, Extension, find_packages

setup(name="cdindex",
    version="1.0.19",
    description="Package for computing the cdindex.",
    author="Russell J. Funk",
    author_email="russellfunk@gmail.com",
    url="http://www.cdindex.info",
    license="GNU General Public License (GPL)",
    install_requires=['future'],
    ext_modules=[
                  Extension("cdindex._cdindex",
                            ["src/cdindex.cpp", 
                             "src/graph.cpp", 
                             "src/utility.c", 
                             "cdindex/pycdindex.cpp"],
                             include_dirs = ["src"],
                             headers = ["src/cdindex.h"],
                           )
                ],
    packages=find_packages(),
    include_files=['src/cdindex.h']
)

# python setup.py build_ext --inplace
# python setup.py sdist
