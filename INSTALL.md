# Installation
The following can be used to help install datetools into your environment.
This tool is not currently compatible with Microsoft Windows.

## Requirements
You will require the following in order for this to work correctly:
* automake
* autoconf
* libtool
* make
* GNU C++ Compiler
* boost
* python v2.4+ (optional)

## Build Procedure
The following will set a correct build environment
```bash
autogen.sh
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

# Install the package into your system (as root)
# some distributions just allow you to prefix
# the below command with _sudo_
make install
```

You're done!
