cd ..
mkdir Solution
cd Solution
cmake ../ -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=CMakeConfig/Toolchains/Boards/STM32F4Discovery/Toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=CMakeConfig/Toolchains/arm-none-eabi/make-3.81-bin/bin/make.exe
make
cd ..
cd Tools
