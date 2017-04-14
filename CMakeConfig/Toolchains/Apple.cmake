###################
# Compiler Settings
###################
MESSAGE("Configuration for Linux Platform is called")

if(DEFINED TARGET_TOOLCHAIN)
  if( ${TARGET_TOOLCHAIN} STREQUAL "arm-none")
    set(CMAKE_SYSTE_NAME            Generic)
    set(CMAKE_SYSROOT               /usr/bin )
    set(CMAKE_SYSTEM_PROCESSOR      cortex-m3 )
    set(CMAKE_C_COMPILER            "/usr/bin/arm-none-eabi-gcc")
    set(CMAKE_CXX_COMPILER          "/usr/bin/arm-none-eabi-g++")
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
  end()
endif()
  
