# Installation
The following can be used to help install datetools into your environment.
This tool is not currently compatible with Microsoft Windows.

# Requirements
You will require the following in order for this to work correctly:
* automake
* autoconf
* libtool
* make
* GNU C++ Compiler
* boost
* python v2.4+ (optional)

The following will set a correct build environment
```bash
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

automake --add-missing --copy

autoconf
```

If you dont want the python module (optionally over-ride the prefix):
```bash
./configure --prefix=`pwd` --without-python
```

Otherwise run the following:
```bash
# Configure the environment
./configure

# Compile the code
make

# Install the package into your system
make install
```

You're done!
