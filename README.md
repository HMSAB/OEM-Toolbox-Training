# OEM-Toolbox-Training
Get to know the OEM toolbox with this gudided tour. 


notes:

dependencies:

libjansson http://www.digip.org/jansson/


uuid-dev


Create a build folder and go into it:
$ mkdir build
$ cd build

CMake:
$ cmake ../ -DCMAKE_TOOLCHAIN_FILE=../toolchainNB.cmake

Compile:
$ make