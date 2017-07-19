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
include( VERSION.cmake )
include( CMakeConfig/Macros.cmake )

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
# Load all Projects
###################
include(CcOS.cmake)

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
