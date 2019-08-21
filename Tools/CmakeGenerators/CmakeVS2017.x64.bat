SET TOOLS_DIR=%cd%
cd ..
cd ..

mkdir Solution.VC15.x64
cd Solution.VC15.x64
cmake ../ -G "Visual Studio 15 Win64"

cd "%TOOLS_DIR%"
