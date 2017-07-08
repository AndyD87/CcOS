cd ..
mkdir Solution.CB
cd Solution.CB
cmake -G "CodeBlocks - Unix Makefiles" ../ -DCCOS_BOARD=CMakeConfig/Boards/STM32F4Discovery/Config.cmake -DCMAKE_BUILD_TYPE=Release
cd ..
cd Tools

cd ..
mkdir Solution.ECL
cd Solution.ECL
cmake -G "Eclipse CDT4 - Unix Makefiles" ../ -DCCOS_BOARD=CMakeConfig/Boards/STM32F4Discovery/Config.cmake -DCMAKE_BUILD_TYPE=Release
cd ..
cd Tools
