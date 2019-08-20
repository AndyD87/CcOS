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

# Enable to check memory usage and leakings
#set(MEMORYMONITOR_ENABLED TRUE)

# Set Debug build if not BUILD_TYPE selected
#if(NOT CMAKE_BUILD_TYPE)
#  set(CMAKE_BUILD_TYPE Debug)
#endif()


# Enable verbose output if required
#set(CMAKE_VERBOSE_MAKEFILE TRUE CACHE INTERNAL "")

message("-- Default build type: ${CMAKE_BUILD_TYPE}")

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
  if(NOT DEFINED CC_CACHE_DIR)
    set( CC_CACHE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Cache)
    if(NOT EXISTS ${CC_CACHE_DIR})
      file(MAKE_DIRECTORY ${CC_CACHE_DIR})
    endif()
  endif()
  add_subdirectory( ${CCOS_BOARD} )
endif()

################################################################################
# Setup Output Directory if not set
################################################################################
# To be able to overwrite global output root use -DCC_OUTPUT_DIR=/path on configuration
if(NOT CC_OUTPUT_DIR)
  set(CC_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Output")
endif()

if(CCOS_CMAKE_INCLUDES)
  list(REMOVE_DUPLICATES CCOS_CMAKE_INCLUDES)
  foreach(CCOS_CMAKE_INCLUDE ${CCOS_CMAKE_INCLUDES})
    include(${CCOS_CMAKE_INCLUDE})
  endforeach()
endif()

################################################################################
# Configuration is done, setup Project
################################################################################
if(NOT CCOS_NO_PROJECT)
  project("CcOS" VERSION "${CCOS_VERSION_CMAKE}")
endif()

message("- Compiler-ID: ${CMAKE_CXX_COMPILER_ID}")

################################################################################
# Setup Default Values for CMAKE
################################################################################
CcNoConfigurationDirs()
enable_testing()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Load all Projects
################################################################################
include(${CMAKE_CURRENT_SOURCE_DIR}/CcOS.cmake)

################################################################################
# Do not build apps for generic platform
################################################################################
if(NOT GENERIC AND
   NOT CCOS_NO_APPS)
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

  include(CPack)
elseif(WINDOWS)
  CcLoadWixTools()

  set(CPACK_GENERATOR "WIX")
  set(CPACK_WIX_UPGRADE_GUID "5E8C2805-2CA7-47DD-B250-0F62434F3360")
  include(CPack)
endif()
