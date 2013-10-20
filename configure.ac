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
AC_PREREQ(2.59)
AC_INIT(datetools, 0.8.0, lead2gold@gmail.com)

AC_CONFIG_HEADERS(src/config.h)

AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_PROG_CXX
AC_STDC_HEADERS
CXXFLAGS="$CXXFLAGS -DCRON_ASSERT_LOOP_LIMIT=400"

AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([boost/program_options.hpp], [],
    [AC_MSG_ERROR(You need the Boost libraries.)])
AC_LANG_POP([C++])

AC_ARG_ENABLE([python],
              AS_HELP_STRING([--disable-python],
                             [don't build Python bindings]),
              [], [enable_python=yes])

AC_ARG_ENABLE([optimize],
              AS_HELP_STRING([--disable-optimize],
                             [don't try to compile with optimizations]),
              [], [enable_optimize=yes])

AC_ARG_VAR([PYTHON], [which Python interpreter to use])


if test "x$enable_python" = "xyes"; then
  AM_PATH_PYTHON([2.6],,
                 [AC_MSG_ERROR([datetools requires Python >= 2.6 to be installed
  -------------------------------------------------------------------
  If you already have Python 2.6 installed, please ensure you have
  not set the PYTHON environment variable to point to an older
  version of Python.
  -------------------------------------------------------------------
  ])])

  AC_SUBST([PYTHON_EGG_POSTFIX])
  AC_SUBST([PYTHON_EGG_PUREPY_POSTFIX])
  AC_SUBST([PYTHON]) # Used by the example shell scripts and src/Makefile.am.
  has_python=yes
fi
AM_CONDITIONAL([HAS_PYTHON], [test "x$has_python" = "xyes"])

# Check if we should try and enable optimizations.
if test "x$enable_optimize" = "xyes"; then
  # For now, we only turn on optimizations for gcc.
  if test "x$GCC" = "xyes"; then
    CXXFLAGS="$CXXFLAGS -g2 -O2"
  fi
fi

LDFLAGS="$LDFLAGS -lboost_program_options -lstdc++"

LT_INIT

AC_CONFIG_FILES(Makefile src/Makefile man/Makefile src/setup.py)
AC_OUTPUT
