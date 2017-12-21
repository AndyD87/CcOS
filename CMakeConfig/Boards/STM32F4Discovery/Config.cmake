###
# Setup Toolchain if required
###
if(NOT CMAKE_TOOLCHAIN_FILE)
    message("- CONIGURE Toolchain")
    set(CCOS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(CCOS_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/arm-none-eabi/Toolchain.cmake")
    set(CCOS_TOOLCHAIN_FILE_CONFIGURED "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/arm-none-eabi/Toolchain.configured.cmake")
    configure_file(${CCOS_TOOLCHAIN_FILE} ${CCOS_TOOLCHAIN_FILE_CONFIGURED} @ONLY)
    set(CMAKE_TOOLCHAIN_FILE ${CCOS_TOOLCHAIN_FILE_CONFIGURED})
    message("- Toolchain-File: ${CMAKE_TOOLCHAIN_FILE}")
endif()

#
# Setup CPU-Config
#
include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/CPU/ST/STM32F4/Config.cmake")
