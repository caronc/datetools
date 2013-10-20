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
AutoReqProv: no
%{!?python_sitearch: %global python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(1))")}
Summary: A program allowing you to mimic cron based time calculations
Group: Applications/File
License: GPLv2+
Name: datetools
Release: 1%{?dist}
Version: 0.8.0
Source: %{name}-%{version}.tar.gz
Packager: Chris Caron <lead2gold@gmail.com>
URL: http://www.lead2gold.org
Buildroot: %{_tmppath}/%{name}-%{version}-root
Requires: boost, libstdc++
Provides: datetools
BuildRequires: glib-devel
BuildRequires: boost-devel, libstdc++-devel, python-devel
# Nice to have available:
BuildRequires: autoconf, automake, libtool
%description
Dateblock gives the functionality of a cron job from the command line.
Simliar to sleep; the tool will block until a specified date/time is
reached (oppose to sleep which only blocks for a defined number of seconds)

%package -n dateblock-python
Group: Applications/File
Summary: Provides python bindings for dateblock (cron calulations)
Requires: python, boost
%description -n dateblock-python
Provides python bindings for dateblock

%package -n dateblock
Group: Applications/File
Summary: command line tool for handling crontab calculations and blocking
Requires: boost
%description -n dateblock
Dateblock allows command line blocking and calculations based on crontab
input.

%package -n datemath
Group: Applications/File
Summary: command line tool for handling date manipulation
Requires: boost
%description -n datemath
Datemath allows you to perform math on the current date

%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
%makeinstall

%clean
rm -rf $RPM_BUILD_ROOT
make clean

%files
%doc README COPYING ChangeLog

%files -n datemath
%attr(0755,root,root) %{_bindir}/datemath
%{_mandir}/man1/datemath.*

%files -n dateblock
%attr(0755,root,root) %{_bindir}/dateblock
%{_mandir}/man1/dateblock.*

%files -n dateblock-python
%attr(-,root,root) %{python_sitearch}/*

%changelog
* Thu Oct 18 2013 Chris Caron <lead2gold@gmail.com> 0.8.0-1
- Initial RPM release
