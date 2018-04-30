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
# 
bin_PROGRAMS=dateblock datemath
#bin_PROGRAMS=dateblock datemath unittest
#unittest_SOURCES=Date.cpp unittest.cpp
dateblock_SOURCES=Date.cpp dateblock.cpp
datemath_SOURCES=Date.cpp datemath.cpp

if HAS_PYTHON
install-exec-local:
	echo "enter install-exec-local in Makefile.am"
	[ ! -z "$(DESTDIR)" ] && $(PYTHON) setup.py install --prefix="$(prefix)" --root="$(DESTDIR)" || :
	[ -z "$(DESTDIR)" ] && $(PYTHON) setup.py install --prefix="$(prefix)" || :
	echo "leave install-exec-local in Makefile.am"

uninstall-local:
	echo "enter uninstall-local in Makefile.am"
	[ ! -z "$(DESTDIR)" ] && $(PYTHON) setup.py install --prefix="$(prefix)" --root="$(DESTDIR)" --record python-files.tmp || :
	[ -z "$(DESTDIR)" ] && $(PYTHON) setup.py install --prefix="$(prefix)" --record python-files.tmp || :
	cat python-files.tmp | xarg rm -rf
	rm -f python-files.tmp
	echo "leave uninstall-local in Makefile.am"

build-python:
	$(PYTHON) setup.py build

# Make sure the python module is built via 'make all'.
all-local: build-python
endif #HAS_PYTHON

clean-python:
	rm -rf build dateblock.so

PHONY_TARGETS = clean-python

clean-local: clean-python

.PHONY: $(PHONY_TARGETS)
