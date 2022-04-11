# CcOS

Target of this framework is to get a base for developing simple application, wich will work independent from Linux or Windows.
Additional and future target is to expand this framework for ARM Devices as Operating System.

The framework is under construction and not recommended for productive use.

A little overview:
* Language: c++11
* Buildsystem: cmake
* BugTracker: comming soon

Current status:
 * [![Build Status](https://travis-ci.org/AndyD87/CcOS.svg?branch=master)](https://travis-ci.org/AndyD87/CcOS)

## Build on Windows

For building on Windows, at least Visual Studio 2017 is required, earlier Version doesn't fully support c++11.
In subdirectory "Tools", you can find some scripts for creating Solutions depending on Version and Architecture.
Direct builds with nmake are possbile. Look at cmake documentation, how to make it.

Execute following to get a Solution:

    mkdir Solution
    cd Solution
    cmake ..
    CcOS.sln
    
## Build on Linux

One way to build on Linux is to use building scripts under Tools:

    cd Tools
    build.sh # change to "builDebug.sh" if debug is requried
    cd ..\Solution

Second way is to build on commandline, without build script.
Here an example:

    mkdir Solution
    cd Solution
    cmake ../
    make
    make install
  
## Build on Linux (cross compile)

comming soon

## Testing

CcOS is designed to test it self by using CTest from cmake.

Every Solution will create a project called "RUN_TESTS" wich can be build or executed to run all tests.

If CcOS CmakeList was build with the environment variable TEST_CCOS=TRUE, all tests will additionaly check for memory leaks too.  
"Test-All.ps1" and "Test-All.sh" for Windows and Linux will enable this variable by default.

## License

Author of CcOS-Framework: [Andreas Dirmeier](http://adirmeier.de)
This framework is licensed under LGPL v3. Look at COPYING and COPYING.LESSER for further information.
