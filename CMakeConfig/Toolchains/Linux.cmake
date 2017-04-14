###################
# Compiler Settings
###################
MESSAGE("Configuration for Linux Platform is called")

if(DEFINED TARGET_TOOLCHAIN)
  if( ${TARGET_TOOLCHAIN} STREQUAL "arm-none")
    MESSAGE("gnu arm-none choosen")
    set(CMAKE_SYSTEM_NAME Generic)
    CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
    CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
  endif()
  if( ${TARGET_TOOLCHAIN} STREQUAL "arm-linux-gnueabihf")
    MESSAGE("gnu arm-linux-gnueabihf choosen")
    set( CMAKE_SYSTEM_NAME  Linux )
    set( CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
    set( CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
    set(CMAKE_FIND_ROOT_PATH /usr/arm-linux-gnueabihf)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
  endif()
endif()

# use std::11 as basic !
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=c11  ")#  -static-libgcc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")#  -static-libstdc++")

set(LINK_TYPE SHARED)
  
set( PROGRAM_DATA_DIR   "/var/CcOS" )
set( PROGRAM_CONFIG_DIR "/etc/CcOS" )
set( PROGRAM_BIN_DIR    "/bin" )
set( PROGRAM_LIB_DIR    "/lib" )
