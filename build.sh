#!/bin/sh
# Datetools provide a method of manipulating and working dates and times.
# Copyright (C) 2013 Chris Caron <lead2gold@gmail.com>
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

# Cleanup old files
rm -rf autom4te.cache \
    config.h.in config.h.in~ \
    configure config.guess config.sub \
    install-sh depcomp missing Makefile.in \
    aclocal.m4 src/config.h.in src/Makefile \
    Makefile stamp-h1 config.status config.h \
    config.log src/config.h src/stamp-h1 \
    src/.deps src/*.o src/*.a src/core \
    src/Makefile.in src/build src/dateblock \
    src/unittest

#aclocal -I m4
aclocal

ltarg="-i"
if test -f "`which glibtoolize`" ; then
	ltver=`glibtoolize --version | sed -n '1s/.* \([0-9]\).*/\1/p'`
	if test "${ltver}" = 1 ; then ltarg="" ; fi
	echo "glibtoolize -c -f $ltarg"
	glibtoolize -c -f $ltarg
else
	ltver=`libtoolize --version | sed -n '1s/.* \([0-9]\).*/\1/p'`
	if test "${ltver}" = 1 ; then ltarg="" ; fi
	echo "libtoolize -c -f $ltarg"
	libtoolize -c -f $ltarg
fi

autoheader
#autoheader constructs config.h.in

automake --add-missing --copy
autoconf

# at this point the rest of the build process is identical as if you
# had just extracted a dateblock-1.8.tar.gz file

# the configure, make, make install may be executed in this directory
# or a completely different build directory. Examples for this directory
# are:
# if you dont want the python module (optionally over-ride the prefix):
#	./configure --prefix=`pwd` --without-python
#
# otherwise this is fine:
#	./configure

# make
# make install

###########
