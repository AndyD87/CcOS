###
# Setup Toolchain if required
###
if(NOT CMAKE_TOOLCHAIN_FILE)
    message("- Load toolchain: gcc-arm-none-eabi")
    include(${CMAKE_CURRENT_SOURCE_DIR}/../../Toolchains/gcc-arm-none-eabi/Toolchain.cmake)
endif()

# Setup CPU-Config
include(${CMAKE_CURRENT_SOURCE_DIR}/../../CPU/ST/STM32F4/Config.cmake)
