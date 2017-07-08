cd ..
mkdir Solution
cd Solution
cmake -G "Unix Makefiles" ../ -DCCOS_BOARD=CMakeConfig/Boards/STM32F4Discovery/Config.cmake -DCMAKE_BUILD_TYPE=Release
make
cd ..
cd Tools
