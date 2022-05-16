################################################################################
# @Author Andreas Dirmeier
#
# Main File to build all CcOS Components
#
# To Generate a Solution execute the command like this:
#   cmake -G "Visual Studio 14"
#
################################################################################
cmake_minimum_required (VERSION 3.4)

set(CCOS_PROJECT       TRUE)
set(CMAKE_PROJECT_NAME "CcOS")

if($ENV{TEST_CCOS})
  set(MEMORYMONITOR_ENABLED      TRUE)
  set(MEMORYMONITOR_CHECK_KERNEL TRUE)
else()
  # Uncomment to check memory usage and leakings
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
  if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.12)
    set(PAGE_SUPPORT_NAME  HOMEPAGE_URL)
    set(PAGE_SUPPORT_VALUE ${CCOS_URL})
  else()
    set(PAGE_SUPPORT_NAME  )
    set(PAGE_SUPPORT_VALUE )
  endif()
  if(NOT CMAKE_INSTALL_CONFIG_NAME)
    set(CMAKE_INSTALL_CONFIG_NAME ${CMAKE_BUILD_TYPE})
  endif()
  project(${CMAKE_PROJECT_NAME}
          VERSION       ${CCOS_VERSION_CMAKE} # Value is defined in VERSION.cmake
          LANGUAGES     ${CCOS_LANGUAGES}     # Value is defined in VERSION.cmake
          DESCRIPTION   ${CCOS_DESCRIPTION}   # Value is defined in VERSION.cmake
          ${PAGE_SUPPORT_NAME} ${PAGE_SUPPORT_VALUE}  # Value is defined in VERSION.cmake
  )
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
if(WINDOWS OR LINUX)
  ################################################################################
  # Add Testing-Apps too
  ################################################################################
  if(EXISTS  ${CMAKE_CURRENT_SOURCE_DIR}/Testing)
    add_subdirectory(Testing)
  endif()
else()
  add_subdirectory(Testing/CcTestCli)
endif()

################################################################################
# Update dokumentation
################################################################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Documents/Doxygen)

add_custom_target(${CMAKE_PROJECT_NAME})

# generate and install export file
CcTargetFiles(${CMAKE_PROJECT_NAME} CMakeConfig/CcMacros.cmake)
CcExportFiles(CMakeConfig/FindCcOS.cmake)

set(CPACK_PACKAGE_DESCRIPTION         ${CMAKE_PROJECT})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CcOS Framework")
set(CPACK_PACKAGE_VENDOR              "Andreas Dirmeier")
set(CPACK_PACKAGE_CONTACT             "coolcow_ccos@yahoo.com")
set(CPACK_PACKAGE_VERSION             ${PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY   ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_DIRECTORY           ${CMAKE_CURRENT_SOURCE_DIR}/Packs)
set(CPACK_RESOURCE_FILE_LICENSE       ${CMAKE_CURRENT_SOURCE_DIR}/Documents/COPYING.LESSER.rtf)
set(CPACK_ARCHIVE_COMPONENT_INSTALL   "ON")

if(LINUX)
  set(CPACK_GENERATOR                   "DEB;ZIP")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER   ${CPACK_PACKAGE_VENDOR})
  set(CPACK_DEB_COMPONENT_INSTALL       "ON")
  string(TOLOWER ${CC_BUILD_ARCH}       CC_BUILD_ARCH_LOWER)
  string(TOLOWER ${CC_LINK_TYPE}        CC_LINK_TYPE_LOWER)
  string(TOLOWER ${CMAKE_BUILD_TYPE}    CMAKE_BUILD_TYPE_LOWER)
  set(CPACK_SYSTEM_NAME                 "${CMAKE_SYSTEM_NAME}-${CC_COMPILER}-${CC_BUILD_ARCH_LOWER}-${CC_LINK_TYPE_LOWER}-${CMAKE_BUILD_TYPE_LOWER}")

  include(CPack)
elseif(WINDOWS)
  CcVisualStudioPostFix(VS_POSTFIX "$\\\{CPACK_BUILD_CONFIG\\\}" ${CC_LINK_TYPE} ${CC_LINK_TYPE})
  set(CPACK_SYSTEM_NAME                 ${VS_POSTFIX})
  set(CPACK_GENERATOR                   "ZIP")

  include(CPack)
endif()
