
TOOLS_DIR=$(pwd)
cd ..
SOLUTION_POSTFIX=".Solution.ECL.Testing"
TARGET_DIR=$(pwd)
TARGET_DIR_FIXED="$TARGET_DIR$SOLUTION_POSTFIX"

rm -rf "$TARGET_DIR_FIXED"
mkdir "$TARGET_DIR_FIXED"
cd "$TARGET_DIR_FIXED"
cmake -G "Eclipse CDT4 - Unix Makefiles" "$TOOLS_DIR/.." -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=CMakeConfig/Boards/espressif/ESP8266 -DCMAKE_BUILD_TYPE=Debug
if [ $? -ne 0 ]
then
    exit -1
fi

make -j $CPU
if [ $? -ne 0 ]
then
    exit -1
fi

# Test not possible for generic device
#make test
#if [ $? -ne 0 ]
#then
#    exit -1
#fi

cd "$TOOLS_DIR"
rm -rf "$TARGET_DIR_FIXED"
mkdir "$TARGET_DIR_FIXED"
cd "$TARGET_DIR_FIXED"

cmake -G "Eclipse CDT4 - Unix Makefiles" "$TOOLS_DIR/.." -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=CMakeConfig/Boards/espressif/ESP8266 -DCMAKE_BUILD_TYPE=Release
if [ $? -ne 0 ]
then
    exit -1
fi

make -j $CPU
if [ $? -ne 0 ]
then
    exit -1
fi

# Test not possible for generic device
#make test
#if [ $? -ne 0 ]
#then
#    exit -1
#fi

cd "$TOOLS_DIR"
rm -rf "$TARGET_DIR_FIXED"
