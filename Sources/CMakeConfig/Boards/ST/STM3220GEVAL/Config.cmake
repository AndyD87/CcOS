###
# Setup Toolchain if required
###
if(NOT CMAKE_TOOLCHAIN_FILE)
    # download debug tools before setting generic type from gcc
    include(${CMAKE_CURRENT_LIST_DIR}/../../../Toolchains/openocd/Toolchain.cmake)
    message("- Load toolchain: gcc-arm-none-eabi")
    include(${CMAKE_CURRENT_LIST_DIR}/../../../Toolchains/gcc-arm-none-eabi/Toolchain.cmake)
endif()

set(CCOS_BOARD_TYPE "STM3220GEVAL")
set(CCOS_CPU_TYPE 	"STM32F207IG")

# Setup CPU-Config
include(${CMAKE_CURRENT_LIST_DIR}/../../../CPU/ST/STM32F2/Config.cmake)
