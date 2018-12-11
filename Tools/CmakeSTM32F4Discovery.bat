cd ..
mkdir Solution.ECL
cd Solution.ECL
cmake -G "Eclipse CDT4 - Ninja" ../ -DCCOS_BOARD=CMakeConfig/Boards/STM32F4Discovery -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=E:/0-Tools/Git/CcOS/Cache/Toolchains/Ninja/ninja.exe
cd ..
cd Tools

cd ..
mkdir Solution.CBX
cd Solution.CBX
cmake -G "CodeBlocks - Ninja" ../ -DCCOS_BOARD=CMakeConfig/Boards/STM32F4Discovery -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=E:/0-Tools/Git/CcOS/Cache/Toolchains/Ninja/ninja.exe
cd ..
cd Tools
