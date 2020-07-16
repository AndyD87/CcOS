################################################################################
# @Author Andreas Dirmeier
#
# Main File to build all CcOS Components
#
# To Generate a Solution execute the command:
#   cmake -G "Visual Studio 14"
#
################################################################################
cmake_minimum_required (VERSION 3.0)

set(CCOS_PROJECT TRUE)

if($ENV{TEST_CCOS})
  set(MEMORYMONITOR_ENABLED      TRUE)
  set(MEMORYMONITOR_CHECK_KERNEL TRUE)
else()
  # Enable to check memory usage and leakings
  #set(MEMORYMONITOR_ENABLED      TRUE)
  #set(MEMORYMONITOR_CHECK_KERNEL TRUE)
  #add_definitions(-DTESTING_REDUCE_STRESS)
endif()

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
include(${CMAKE_CURRENT_LIST_DIR}/CcOS.config.cmake)

################################################################################
# Setup Cache directory if not yet defined
################################################################################
if(NOT DEFINED CC_CACHE_DIR)
  # Check for global Cache dir
  if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../Cache)
	  set( CC_CACHE_DIR ${CMAKE_CURRENT_LIST_DIR}/../Cache)
  else()
    # Check for local Cache dir
    set( CC_CACHE_DIR ${CMAKE_CURRENT_LIST_DIR}/Cache)
    if(NOT EXISTS ${CC_CACHE_DIR})
      file(MAKE_DIRECTORY ${CC_CACHE_DIR})
    endif()
  endif()
endif()

################################################################################
# Setup Output Directory if not set
################################################################################
# To be able to overwrite global output root use -DCC_OUTPUT_DIR=/path on configuration
if(NOT CC_OUTPUT_DIR)
  set(CC_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Output")
endif()

################################################################################
# Configuration is done, setup Project
################################################################################
if(NOT CC_NO_PROJECT)
  project("CcOS" VERSION "${CCOS_VERSION_CMAKE}" )
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
if(NOT GENERIC)
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
