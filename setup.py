#!/usr/local/bin/python
# -*- coding: utf-8 -*-

"""setup.py: This script installs the cdindex python module."""

__author__ = "Russell J. Funk and Diomidis Spinellis"
__copyright__ = "Copyright (C) 2019, 2023"

# built in modules
from setuptools import setup, Extension, find_packages

setup(name="fast_cdindex",
    version="1.1.0",
    description="Package for quickly computing the cdindex.",
    author=["Russell J. Funk", "Diomidis Spinellis"],
    author_email=["russellfunk@gmail.com", "dds@aueb.gr"],
    url="https://github.com:dspinellis/cdindex",
    license="GNU General Public License (GPL)",
    install_requires=['future'],
    ext_modules=[
                  Extension("fast_cdindex._cdindex",
                            ["src/cdindex.cpp", 
                             "fast_cdindex/pycdindex.cpp"],
                             include_dirs = ["src"],
                             headers = ["src/cdindex.h"],
                           )
                ],
    packages=find_packages(),
    include_files=['src/cdindex.h']
)

# python setup.py build_ext --inplace
# python setup.py sdist
