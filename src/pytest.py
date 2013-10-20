#!/usr/bin/env python
# -*- coding: utf-8 -*-
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
"""
This is just a simple tool for testing that the output is as expected
"""
from datetime import datetime
from dateblock import dateblock
from datetime import date
from datetime import time
# Support python datetime object
print dateblock("*/1", ref=datetime(2000, 5, 3, 10, 10, 0))
# Support python date object
print dateblock("*/1", ref=date(2000, 5, 3))
# Support python time object
print dateblock("*/1", ref=time(20, 5, 3), block=False)
# Time equals 'now'
print dateblock("*/1", ref=None, block=False)
# Epoch Time
print dateblock("*/1", ref=7999434323, block=False)
# Drift Time
print dateblock("*/10", ref=7999434323, block=False, drift=5)
# Blocking should never be possible if the time is in the past
print dateblock("*/10", ref=999434323, block=True, drift=7)
