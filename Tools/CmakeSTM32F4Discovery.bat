cd ..
mkdir Solution.ECL
cd Solution.ECL
cmake -G "Eclipse CDT4 - Unix Makefiles" ../ -DCCOS_BOARD=CMakeConfig/Boards/STM32F4Discovery -DCMAKE_BUILD_TYPE=Release
cd ..
cd Tools

cd ..
mkdir Solution.CBX
cd Solution.CBX
cmake -G "CodeBlocks - Ninja" ../ -DCCOS_BOARD=CMakeConfig/Boards/STM32F4Discovery -DCMAKE_BUILD_TYPE=Release
cd ..
cd Tools
