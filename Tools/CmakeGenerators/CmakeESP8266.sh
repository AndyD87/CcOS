TOOLS_DIR=$(pwd)
cd ..
cd ..
SOLUTION_POSTFIX=".Solution.ECL"
TARGET_DIR=$(pwd)
TARGET_DIR_FIXED="$TARGET_DIR$SOLUTION_POSTFIX"

#rm -rf "$TARGET_DIR_FIXED"
mkdir "$TARGET_DIR_FIXED"
cd "$TARGET_DIR_FIXED"
export PATH=$PATH:$TOOLS_DIR/../../Cache/Toolchains/esp-open-sdk/1.22.0.0/xtensa-lx106-elf/bin
export IDF_PATH=$TOOLS_DIR/../../Cache/Sources/espressif/ESP8266_RTOS
cmake -G "Eclipse CDT4 - Unix Makefiles" "$TOOLS_DIR/.." -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=CMakeConfig/Boards/espressif/ESP8266 -DCMAKE_BUILD_TYPE=Debug
if [ $? -ne 0 ]
then
    exit -1
fi

echo export PATH=$PATH
echo export IDF_PATH=$IDF_PATH
