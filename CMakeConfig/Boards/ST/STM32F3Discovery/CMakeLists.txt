###
# Setup Toolchain if required
###
if(NOT CMAKE_TOOLCHAIN_FILE)
    # download debug tools before setting generic type from gcc
    include(${CMAKE_CURRENT_LIST_DIR}/../../../Toolchains/openocd/Toolchain.cmake)
    message("- Load toolchain: gcc-arm-none-eabi")
    include(${CMAKE_CURRENT_LIST_DIR}/../../../Toolchains/gcc-arm-none-eabi/Toolchain.cmake)
endif()

# Setup CPU-Config
include(${CMAKE_CURRENT_LIST_DIR}/../../../CPU/ST/STM32F3/Config.cmake)

set(CCOS_DRIVER_BOARD                     TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_BOARD_ST                  TRUE CACHE INTERNAL "")
set(CCOS_DRIVER_BOARD_ST_STM32F3DISCOVERY TRUE CACHE INTERNAL "")
