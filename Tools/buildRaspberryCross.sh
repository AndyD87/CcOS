cd ..
mkdir Solution
cd Solution
cmake ../ -DCMAKE_TOOLCHAIN_FILE=CMakeConfig/Toolchains/Raspberry/ToolchainFile.cmake -DCONFIGFILE=../CMakeConfig/Configs/Config.Raspberry.cmake -DCMAKE_BUILD_TYPE=Release
make
cd ..
cd Tools
