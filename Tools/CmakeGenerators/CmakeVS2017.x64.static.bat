SET TOOLS_DIR=%cd%
cd ..
cd ..

mkdir Solution.VC15.x64.static
cd Solution.VC15.x64.static
cmake ../ -G "Visual Studio 15 Win64" -DCC_LINK_TYPE=STATIC

cd "%TOOLS_DIR%"
