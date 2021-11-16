message("- Crosscompiling Toolchain activated")
set( CMAKE_SYSTEM_NAME  Linux )
set( LINARO_GCC_DIR "${CC_CACHE_DIR}/Toolchains/gcc-linaro-arm-linux-gnueabihf" )

CcDownloadAndExtract( "linaro-gcc-arm-none-eabi"
                      "${LINARO_GCC_DIR}"
                      "https://releases.linaro.org/archive/13.04/components/toolchain/binaries/gcc-linaro-arm-linux-gnueabihf-4.7-2013.04-20130415_linux.tar.xz")
CcLoadMakeProgram()

set( LINARO_GCC_BIN_DIR "${LINARO_GCC_DIR}/gcc-linaro-arm-linux-gnueabihf-4.7-2013.04-20130415_linux/bin" )
# specify the cross compiler
set(CMAKE_ASM_COMPILER ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-as     CACHE INTERNAL "")
set(CMAKE_C_COMPILER   ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-gcc    CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-g++    CACHE INTERNAL "")
set(CMAKE_AR           ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-ar     CACHE INTERNAL "")
set(GCC_SIZE           ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-size   CACHE INTERNAL "")

# use sysroot from minimal build at
#  https://github.com/armbian/build.git
#   ./compile.sh \
#     BOARD=orangepizero \
#     BRANCH=current \
#     RELEASE=focal \
#     BUILD_MINIMAL=yes \
#     BUILD_DESKTOP=no \
#     KERNEL_ONLY=no \
#     KERNEL_CONFIGURE=no \
#     CARD_DEVICE="/dev/sda"

set(CMAKE_FIND_ROOT_PATH ${LINARO_GCC_BIN_DIR})

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER             CACHE INTERNAL "")
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY              CACHE INTERNAL "")
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY              CACHE INTERNAL "")
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY              CACHE INTERNAL "")
