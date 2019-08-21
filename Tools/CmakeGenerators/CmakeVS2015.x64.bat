SET TOOLS_DIR=%cd%
cd ..
cd ..

mkdir Solution.VC14.x64
cd Solution.VC14.x64
cmake ../ -G "Visual Studio 14 Win64"

cd "%TOOLS_DIR%"
