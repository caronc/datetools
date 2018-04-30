#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Datetools provide a method of manipulating and working dates and times.
# Copyright (C) 2013-2018 Chris Caron <lead2gold@gmail.com>
#
# This file is part of Datetools.  Datetools is free software; you can
# redistribute it and/or modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

try:
    from setuptools import setup

except ImportError:
    from distutils.core import setup

from distutils.core import Extension

setup(
    name = "dateblock",
    version = "1.0.2",
    url='https://github.com/caronc/datetools/',
    long_description=open('README.md').read(),
    description="Python Cron-like date mainpulation",
    author='Chris Caron',
    author_email='lead2gold@gmail.com',
    license="GPLv2",
    classifiers=(
                'Development Status :: 5 - Production/Stable',
                'Intended Audience :: Developers',
                'Operating System :: POSIX :: Linux',
                'Natural Language :: English',
                'Programming Language :: C++',
                'Programming Language :: Python',
                'Programming Language :: Python :: 2.7',
                'License :: OSI Approved :: GNU General Public License v2 (GPLv2)',
            ),
    ext_modules = [
        Extension("dateblock", ["src/Date.cpp", "src/dateblock.cpp"],
                  libraries=['stdc++'],
                  define_macros=[
                      ('PYTHON_MODULE', '1'),
                  ])],
    keywords='date sleep block time manipulation cron',
)
