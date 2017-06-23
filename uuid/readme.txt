Build steps - x86
=================

Create a build folder and go into it:
$ mkdir build
$ cd build

CMake:
$ cmake ../

Compile:
$ make


Build steps - embedded
======================

Source the sdk file to setup your environment:
$ source </path/to/sdk>/environment-setup-cortexa8hf-vfp-neon-oe-linux-gnueabi

Create a build folder and go into it:
$ mkdir build
$ cd build

CMake:
$ cmake ../ -DCMAKE_TOOLCHAIN_FILE=../toolchainNB.cmake

Compile:
$ make


Testing -- 
========================
There is a single unit test that is currently completed manually. This program will read MAC address from an input file,
produce type 3 UUIDs based on the MAC and ID, and then test that the UUID is unique. 

to run: 
./test_uuid_gen ../testmac.txt

expectation for results: 
 - if there is a dupicate mac on in the file, then this will be identified
 - if all MACs are unique, then there will be 0 output
