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
set(CCOS_CPU_TYPE   "STM32F103C8")
set(CCOS_BOARD_TYPE "STM32F103C8")

include(${CMAKE_CURRENT_LIST_DIR}/../../../CPU/ST/STM32F1/Config.cmake)
