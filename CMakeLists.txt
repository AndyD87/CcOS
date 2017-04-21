# @Author Andreas Dirmeier
#
# Main File to build all CcOS Components
#
# To Generate a Solution execute the command:
#   cmake -G "Visual Studio 12" -DTARGET_SYSTEM="Windows"
#
cmake_minimum_required (VERSION 2.8)
project(Main)

###################
# Set Global Version
###################
include( VERSION.cmake )
# Load Macros
include( CMakeConfig/Macros.cmake )

###################
# Include Config File if config is set
###################
if(DEFINED CONFIGFILE)
  include( ${CONFIGFILE} )
else()
  # Use default System Config
  if(DEFINED APPLE)
    include( CMakeConfig/Configs/Config.Apple.cmake)
  elseif(DEFINED WIN32)
    set(WINDOWS TRUE)
    include( CMakeConfig/Configs/Config.Windows.cmake)
  else()
    set(LINUX TRUE)
    include( CMakeConfig/Configs/Config.Linux.cmake)
  endif()
endif()

###################
# Process compiler flags
###################
if( APPLE )
  include( CMakeConfig/Toolchains/Apple.cmake)
elseif( WIN32 )
  set(CMAKE_BUILD_TYPE "Release")
  include( CMakeConfig/Toolchains/Windows.cmake)
else( LINUX )
  include( CMakeConfig/Toolchains/Linux.cmake)
endif()

###################
# Global Settings:
###################

# Turn on Verbose output on DEBUG

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  message("Build-Type set to ${CMAKE_BUILD_TYPE}")
  set(CCOS_BUILD_TYPE "DEBUG" )
  set(CMAKE_VERBOSE_MAKEFILE          ON)
else()
  message("Build-Type set to release")
  set(CCOS_BUILD_TYPE "RELEASE" )
endif()

set(EXECUTABLE_OUTPUT_PATH            ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set(LIBRARY_OUTPUT_PATH               ${CMAKE_CURRENT_SOURCE_DIR}/bin )
link_directories( ${EXECUTABLE_OUTPUT_PATH} )
enable_testing()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

###################
# Precheck all configurations and load thirdparty if required
###################
include( CMakeConfig/ConfigCheck.cmake )

###################
# Add CcOS to compile
# Add all CcLib Objects to build-List:
###################
add_subdirectory( CcKernel/ )
add_subdirectory( CcGui/ )
add_subdirectory( CcNetwork/ )
add_subdirectory( CcMedia/ )
add_subdirectory( CcUtil/ )
add_subdirectory( CcApps/ )
add_subdirectory( CcModules/ )

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
