cd ..
mkdir Solution
cd Solution
copy ..\Tools\BuildAndPackage.bat .\
cmake.exe ..\ -G "Visual Studio 14 Win64" -DCCOS_OUTPUT_DIR="..\Temp"
cmake.exe --build . --config Release
SET CWD=%cd%
PATH=%PATH%;%CWD%\..\Cache\Tools\wix-portable
cpack.exe