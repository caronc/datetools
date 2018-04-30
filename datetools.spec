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
%{!?python_sitearch: %global python_sitearch %(%{__python} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib(1))")}
Summary: Date Manipulation Tools
Group: System Environment/Base
License: GPLv2+
Name: datetools
Release: 1%{?dist}.nuxref
Version: 1.0.1
Source: %{name}-%{version}.tar.gz
Packager: Chris Caron <lead2gold@gmail.com>
URL: http://nuxref.com
Buildroot: %{_tmppath}/%{name}-%{version}-root
Requires: boost, libstdc++
Requires: dateblock = %{version}-%{release}
Requires: datemath = %{version}-%{release}
Requires: boost-program-options
Provides: datetools
BuildRequires: glib-devel
BuildRequires: boost-devel, gcc-c++, libstdc++-devel, python-devel
# Nice to have available:
BuildRequires: autoconf, automake, libtool

%description
Dateblock gives the functionality of a cron job from the command line.
Simliar to sleep; the tool will block until a specified date/time is
reached (oppose to sleep which only blocks for a defined number of seconds)

%package -n python-dateblock
Group:  Development/Libraries
Summary: Provides python bindings for dateblock (cron calulations)
Requires: python, boost
Obsoletes: dateblock-python
%description -n python-dateblock
Provides python bindings for dateblock

%package -n dateblock
Group: System Environment/Base
Summary: command line tool for handling crontab calculations and blocking
Requires: boost
%description -n dateblock
Dateblock allows command line blocking and calculations based on crontab
input.

%package -n datemath
Group: System Environment/Base
Summary: command line tool for handling date manipulation
Requires: boost
%description -n datemath
Datemath allows you to perform math on the current date

%prep
%setup -q

%build
./autogen.sh
%configure
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
%makeinstall

%clean
rm -rf $RPM_BUILD_ROOT
make clean

%files
%doc AUTHORS README.md COPYING ChangeLog

%files -n datemath
%attr(0755,root,root) %{_bindir}/datemath
%{_mandir}/man1/datemath.*

%files -n dateblock
%attr(0755,root,root) %{_bindir}/dateblock
%{_mandir}/man1/dateblock.*

%files -n python-dateblock
%attr(-,root,root) %{python_sitearch}/*

%changelog
* Mon Apr 30 2018 Chris Caron <lead2gold@gmail.com> 1.0.1-1
- Updated to version 1.0.1

* Fri Apr 21 2017 Chris Caron <lead2gold@gmail.com> 1.0.0-1
- Updated to version 1.0.0

* Fri Jul 17 2015 Chris Caron <lead2gold@gmail.com> 0.8.1-1
- Updated to version 0.8.1

* Thu Oct 18 2013 Chris Caron <lead2gold@gmail.com> 0.8.0-1
- Initial RPM release
