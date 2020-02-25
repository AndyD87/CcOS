####################################################
# Setup Dirs
####################################################
TOOLS_DIR=$(pwd)/..
cd ..
cd ..
SOLUTION_POSTFIX=".Solution.ECL"
TARGET_DIR=$(pwd)
TARGET_DIR_FIXED="$TARGET_DIR$SOLUTION_POSTFIX"
CC_CACHE_DIR=$TOOLS_DIR/../../Cache

#rm -rf "$TARGET_DIR_FIXED"
mkdir "$TARGET_DIR_FIXED"
cd "$TARGET_DIR_FIXED"
export PATH=$PATH:$CC_CACHE_DIR/Toolchains/esp-open-sdk/1.22.0.0/xtensa-lx106-elf/bin
export IDF_PATH=$CC_CACHE_DIR/Sources/espressif/ESP8266_RTOS

cmake -G "Eclipse CDT4 - Unix Makefiles" "$TOOLS_DIR/.." -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=CMakeConfig/Boards/espressif/ESP8266 -DCMAKE_BUILD_TYPE=Debug -DCC_CACHE_DIR="$CC_CACHE_DIR"

echo "# PATHS for console:"
echo export PATH=$PATH
echo export IDF_PATH=$IDF_PATH
echo "# Cmake variables:"
echo CMAKE_C_COMPILER=$CC_CACHE_DIR/Toolchains/esp-open-sdk/1.22.0.0/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc
echo CMAKE_Cxx_COMPILER=$CC_CACHE_DIR/../../Cache/Toolchains/esp-open-sdk/1.22.0.0/xtensa-lx106-elf/bin/xtensa-lx106-elf-g++
echo "# Build directory:"
echo cd $TARGET_DIR_FIXED
