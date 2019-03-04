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
  if(NOT DEFINED CC_CACHE_DIR)
    set( CC_CACHE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Cache)
    if(NOT EXISTS ${CC_CACHE_DIR})
      file(MAKE_DIRECTORY ${CC_CACHE_DIR})
    endif()
  endif()
  add_subdirectory( ${CCOS_BOARD} )
endif()

################################################################################
# Configuration is done, setup Project
################################################################################
project("CcOS" VERSION "${CCOS_VERSION_CMAKE}")

################################################################################
# Setup Output Directory if not set
################################################################################
# To be able to overwrite global output root use -DCC_OUTPUT_DIR=/path on configuration
if(NOT CC_OUTPUT_DIR)
  set(CC_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Output")
endif()

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
  
  include(CPack)
elseif(WINDOWS)
  CcLoadWixTools()
  
  set(CPACK_GENERATOR "WIX")
  set(CPACK_WIX_UPGRADE_GUID "5E8C2805-2CA7-47DD-B250-0F62434F3360")
  include(CPack)
endif()
