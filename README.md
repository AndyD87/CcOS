# CcOS

Target of this framework is to get a base for developing simple application, wich will work independent from Linux or Windows.
Additional and future target is to expand this framework for ARM Devices as Operating System.

The framework is under construction and not recommended for productive use.

A little overview:
* Language: c++11
* Buildsystem: cmake
* BugTracker: comming soon

## Build on Windows

For building on Windows, at least Visual Studio 2013 is required, earlier Version doesn't fully support c++11.
In subdirectory "Tools", you can find some scripts for creating Solutions depending on Version and Architecture.
Direct builds with nmake are possbile. Look at cmake documentation, how to make it.

Execute following to get a Solution:

    cd Tools
    CmakeVS2013.x64.bat
    cd ..\Solution
    Main.sln
    
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

## Licence

Author of CcOS-Framework: [Andreas Dirmeier](http://adirmeier.de)
This framework is licenced under LGPL v3. Look at COPYING and COPYING.LESSER for further information.
