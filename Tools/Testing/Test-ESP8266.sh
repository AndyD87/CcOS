####################################################
# Setup Dirs
####################################################
TOOLS_DIR=$(pwd)/..
cd ..
cd ..
SOLUTION_POSTFIX=".Solution.ECL.Testing"
TARGET_DIR=$(pwd)
TARGET_DIR_FIXED="$TARGET_DIR$SOLUTION_POSTFIX"
CC_CACHE_DIR=$TOOLS_DIR/../../Cache

#rm -rf "$TARGET_DIR_FIXED"
mkdir "$TARGET_DIR_FIXED"
cd "$TARGET_DIR_FIXED"
export PATH=$PATH:$CC_CACHE_DIR/Toolchains/esp-open-sdk/1.22.0.0/xtensa-lx106-elf/bin
export IDF_PATH=$CC_CACHE_DIR/Sources/espressif/ESP8266_RTOS

####################################################
# Prebuild step if it is required
####################################################
if test "1" = "${PREBUILD_REQUIRED}"
then
  # Next step can fail, it is just for loading sdk
  cmake -G "Eclipse CDT4 - Unix Makefiles" "$TOOLS_DIR/.." -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=CMakeConfig/Boards/espressif/ESP8266 -DCMAKE_BUILD_TYPE=Debug -DCC_CACHE_DIR="$CC_CACHE_DIR"
  python3 -V
  if [ $? -ne 0 ]
  then
    sudo -n apt-get install -y python3-pip python3-dev libxml2-dev libxslt-dev
    sudo -n apt-get install -y python3-serial
    sudo -n python3 -m pip install --upgrade pip
    sudo -n python3 -m pip install --upgrade setuptools
    sudo -n python3 -m pip install pyserial
    sudo -n python3 -m pip install future
    sudo -n python3 -m pip install cryptography
    sudo -n python3 -m pip install esptool
  else
    sudo -n apt-get install -y python-pip python-dev libxml2-dev libxslt-dev
    sudo -n python -m pip install --upgrade pip
    sudo -n python -m pip install --upgrade setuptools
    sudo -n python -m pip install pyserial==3.0
    sudo -n python -m pip install future==0.15.2
    sudo -n python -m pip install cryptography==2.1.4
  fi
fi

cmake -G "Eclipse CDT4 - Unix Makefiles" "$TOOLS_DIR/.." -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=CMakeConfig/Boards/espressif/ESP8266 -DCMAKE_BUILD_TYPE=Debug -DCC_CACHE_DIR="$CC_CACHE_DIR"
if [ $? -ne 0 ]
then
    exit -1
fi
# PATHS for console:"
export PATH=$PATH
export IDF_PATH=$IDF_PATH
# Cmake variables:"
CMAKE_C_COMPILER=$CC_CACHE_DIR/Toolchains/esp-open-sdk/1.22.0.0/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc
CMAKE_CXX_COMPILER=$CC_CACHE_DIR/Toolchains/esp-open-sdk/1.22.0.0/xtensa-lx106-elf/bin/xtensa-lx106-elf-g++

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

cmake -G "Eclipse CDT4 - Unix Makefiles" "$TOOLS_DIR/.." -DCMAKE_ECLIPSE_VERSION=4.9 -DCCOS_BOARD=CMakeConfig/Boards/espressif/ESP8266 -DCMAKE_BUILD_TYPE=Release -DCC_CACHE_DIR="$CC_CACHE_DIR"
if [ $? -ne 0 ]
then
    exit -1
fi

make RUN_CMAKE
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
