SET TOOLS_DIR=%cd%
cd ..

mkdir "Solution.MinGW"
cd "Solution.MinGW"
cmake -G "MinGW Makefiles" "../Sources" -DCCOS_BOARD=CMakeConfig/Boards/MinGW -DCMAKE_BUILD_TYPE=Release -DMINGW_VERSION=6 -DCC_LINK_TYPE=STATIC

cd "%TOOLS_DIR%"
