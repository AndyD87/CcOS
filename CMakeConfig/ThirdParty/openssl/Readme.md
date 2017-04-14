# openssl library for CcOS
## Windows
On Windows, openssl is used as prebuild binaries.
In configuration step, cmake will automatically download the, Visual Studio depending, binaries from http://mirror.adirmeier.de .

The binaries can also be build with the containing powershell scripts.

Build-Requirements:
 - Powershell > 3 ( Downloading files requires higher commandlets )
 - Visual Studio 2013/2015
 - cmake

Example build for Visual Studio 2015:

    powershell.exe .\build.msvc120.ps1

After build process, two zip files are available for x86 and x64 builds.

## Linux
On Linux, the find_package from cmake will be used to search for openssl on system.

