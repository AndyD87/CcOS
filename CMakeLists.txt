################################################################################
# @Author Andreas Dirmeier
#
# Main File to build all CcOS Components
#
# To Generate a Solution execute the command:
#   cmake -G "Visual Studio 12"
#
################################################################################
cmake_minimum_required (VERSION 3.0)


################################################################################
# Setup Globals
################################################################################
include( ${CMAKE_CURRENT_SOURCE_DIR}/VERSION.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/CcMacros.cmake )
include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/ProjectMacros.cmake )


################################################################################
# Setup Toolchain globaly
################################################################################
# Load Board settings if defined
if(DEFINED CCOS_BOARD)
  include( ${CCOS_BOARD} )
endif()

################################################################################
# Configuration is done, setup Project
################################################################################
project("CcOS" VERSION "${CCOS_VERSION_CMAKE}")

################################################################################
# Setup Debug
################################################################################
if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  message("Build-Type set to ${CMAKE_BUILD_TYPE}")
  set(CCOS_BUILD_TYPE "DEBUG" )
  set(CMAKE_VERBOSE_MAKEFILE          ON)
else()
  message("Build-Type set to release")
  set(CCOS_BUILD_TYPE "RELEASE" )
endif()

################################################################################
# Setup Output Paths
################################################################################

# To be able to overwrite global output root use -DCCOS_OUTPUT_DIR=/path on configuration
if(NOT CCOS_OUTPUT_DIR)
  set(CCOS_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Output")
endif()

# Set runtime output dir to root/output if no other location was defined
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CCOS_OUTPUT_DIR}/bin")
endif()

# Set library output dir to root/output if no other location was defined
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CCOS_OUTPUT_DIR}/lib")
endif()

# Set archive output dir to root/output if no other location was defined
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CCOS_OUTPUT_DIR}/lib/static")
endif()

CcNoConfigurationDirs()
enable_testing()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Load all Projects
################################################################################
include(${CMAKE_CURRENT_SOURCE_DIR}/CcOS.cmake)

################################################################################
# Add CcOS Apps
################################################################################
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcApps/ )

################################################################################
# Add Testing-Apps too
################################################################################
if(EXISTS  ${CMAKE_CURRENT_SOURCE_DIR}/Testing)
  add_subdirectory(Testing)
endif()

################################################################################
# Update dokumentation
################################################################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Documents/Doxygen)


set(CPACK_PACKAGE_DESCRIPTION         ${CMAKE_PROJECT})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "This Installation includes all required files to link against CcOS")
set(CPACK_PACKAGE_VENDOR              "Andreas Dirmeier")
set(CPACK_PACKAGE_CONTACT             "coolcow_ccos@yahoo.com")
set(CPACK_PACKAGE_INSTALL_DIRECTORY   ${CMAKE_INSTALL_PREFIX})
set(CPACK_RESOURCE_FILE_LICENSE       ${CMAKE_CURRENT_SOURCE_DIR}/Documents/COPYING.LESSER.rtf)

if(LINUX)
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CPACK_PACKAGE_VENDOR})
  
  INCLUDE(CPack)
elseif(WINDOWS)
  CcLoadWixTools()
  
  set(CPACK_GENERATOR "WIX")
  set(CPACK_WIX_UPGRADE_GUID "5E8C2805-2CA7-47DD-B250-0F62434F3360")
  INCLUDE(CPack)
endif()
