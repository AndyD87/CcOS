cd ..
mkdir "Solution.MinGW"
cd "Solution.MinGW"
cmake -G "MinGW Makefiles" ".." -DCCOS_BOARD=CMakeConfig/Boards/MinGW -DCMAKE_BUILD_TYPE=Debug
