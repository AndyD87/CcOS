###
# Setup Toolchain if required
###
if(NOT CMAKE_TOOLCHAIN_FILE)
    # download debug tools before setting generic type from gcc
    include(${CMAKE_CURRENT_LIST_DIR}/../../../Toolchains/openocd/Toolchain.cmake)
    message("- Load toolchain: gcc-arm-none-eabi")
    include(${CMAKE_CURRENT_LIST_DIR}/../../../Toolchains/gcc-arm-none-eabi/Toolchain.cmake)
endif()

set(CCOS_DRIVER_BOARD                     TRUE )
set(CCOS_DRIVER_BOARD_ST                  TRUE )
set(CCOS_DRIVER_BOARD_ST_STM32F4DISCOVERY TRUE )

set(CCOS_CPU_TYPE "STM32F407VGT")

# Setup CPU-Config
include(${CMAKE_CURRENT_LIST_DIR}/../../../CPU/ST/STM32F4/Config.cmake)
