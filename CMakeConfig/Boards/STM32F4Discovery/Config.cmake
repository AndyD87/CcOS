###
# Setup Toolchain if required
###
if(NOT CMAKE_TOOLCHAIN_FILE)
    MESSAGE("- CONIGURE Toolchain")
    SET(CCOS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    SET(CCOS_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/arm-none-eabi/Toolchain.cmake")
    SET(CCOS_TOOLCHAIN_FILE_CONFIGURED "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/arm-none-eabi/Toolchain.configured.cmake")
    CONFIGURE_FILE(${CCOS_TOOLCHAIN_FILE} ${CCOS_TOOLCHAIN_FILE_CONFIGURED} @ONLY)
    SET(CMAKE_TOOLCHAIN_FILE ${CCOS_TOOLCHAIN_FILE_CONFIGURED})
    MESSAGE("- Toolchain-File: ${CMAKE_TOOLCHAIN_FILE}")
endif()

#
# Setup CPU-Config
#
include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/CPU/ST/STM32F4/Config.cmake")
