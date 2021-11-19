message("- Crosscompiling Toolchain for Raspberry activated")
include(${CMAKE_CURRENT_LIST_DIR}/../../../Toolchains/gcc-linaro-arm-linux-gnueabihf/Toolchain.cmake)

set(CCOS_BOARD_TYPE "ORANGEPI_ZERO")
