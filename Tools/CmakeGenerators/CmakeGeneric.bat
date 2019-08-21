SET TOOLS_DIR=%cd%
cd ..
cd ..

mkdir "%cd%.Solution.ECL"
cd "%cd%.Solution.ECL"
cmake -G "Eclipse CDT4 - Unix Makefiles" "%TOOLS_DIR%/.." -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=CMakeConfig/Boards/STM32F4Discovery -DCMAKE_BUILD_TYPE=Debug

cd "%TOOLS_DIR%"
