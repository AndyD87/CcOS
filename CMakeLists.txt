# @Author Andreas Dirmeier
#
# Main File to build all CcOS Components
#
# To Generate a Solution execute the command:
#   cmake -G "Visual Studio 12"
#
cmake_minimum_required (VERSION 2.8)

###################
# Setup Globals
###################
include( ${CMAKE_CURRENT_SOURCE_DIR}/VERSION.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Macros.cmake )

###################
# Setup Toolchain globaly
###################
# Load Board settings if defined
if(DEFINED CCOS_BOARD)
  include( ${CCOS_BOARD} )
endif()

###################
# Configuration is done, setup Project
###################
project("CcOS")

###################
# Setup Debug
###################
if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  message("Build-Type set to ${CMAKE_BUILD_TYPE}")
  set(CCOS_BUILD_TYPE "DEBUG" )
  set(CMAKE_VERBOSE_MAKEFILE          ON)
else()
  message("Build-Type set to release")
  set(CCOS_BUILD_TYPE "RELEASE" )
endif()

# Load Settings depending on Compiler Type
if( APPLE )
  include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/Apple.cmake)
elseif( DEFINED MSVC )
  set(CMAKE_BUILD_TYPE "Release")
  include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/MSVC.cmake)
else( DEFINED GCC )
  include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/GCC.cmake )
endif()

set(EXECUTABLE_OUTPUT_PATH            ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set(LIBRARY_OUTPUT_PATH               ${CMAKE_CURRENT_SOURCE_DIR}/bin )
link_directories( ${EXECUTABLE_OUTPUT_PATH} )
enable_testing()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

###################
# Load all Projects
###################
include(${CMAKE_CURRENT_SOURCE_DIR}/CcOS.cmake)

###################
# Add CcOS Apps
###################
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcApps/ )

###################
# Add Testing-Apps too
###################
if(EXISTS  ${CMAKE_CURRENT_SOURCE_DIR}/Testing)
  add_subdirectory(Testing)
endif()

#set(CPACK_GENERATOR WIX)
#set(CPACK_PACKAGE_NAME "mydll")
#set(CPACK_PACKAGE_VENDOR "CMake.org")
#set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MyLib - CPack Component Installation Example")
#set(CPACK_PACKAGE_VERSION "1.0.0")
#set(CPACK_PACKAGE_VERSION_MAJOR "1")
#set(CPACK_PACKAGE_VERSION_MINOR "0")
#set(CPACK_PACKAGE_VERSION_PATCH "0")
#set(CPACK_PACKAGE_INSTALL_DIRECTORY "CPack Component Example")
#set(CPACK_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION OFF)
#
#INCLUDE(CPack)
#
#EXPORT(PACKAGE CcOS)
