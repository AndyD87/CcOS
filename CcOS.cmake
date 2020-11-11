################################################################################
# Setup Globals
################################################################################
cmake_minimum_required (VERSION 3.4)

# Load configurations
include(${CMAKE_CURRENT_LIST_DIR}/CcOS.config.cmake)

################################################################################
# Include Config File if config is set or auto select
################################################################################
if(DEFINED CONFIGFILE)
  include( ${CONFIGFILE} )
else()
  # Use default System Config
  if("${CMAKE_SYSTEM_NAME}" STREQUAL "Generic" OR GENERIC)
    set(CMAKE_SYSTEM_NAME "Generic")
    message( "- Platform: Generic" )
    include( ${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/Configs/Config.Generic.cmake)
  elseif(DEFINED WIN32)
    message( "- Platform: Windows" )
    include( ${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/Configs/Config.Windows.cmake)
  else()
    message( "- Platform: Linux" )
    include( ${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/Configs/Config.Linux.cmake)
  endif()
endif()

################################################################################
# Load includes if they are available
################################################################################
if(CCOS_CMAKE_INCLUDES)
  list(REMOVE_DUPLICATES CCOS_CMAKE_INCLUDES)
  foreach(CCOS_CMAKE_INCLUDE ${CCOS_CMAKE_INCLUDES})
    include(${CCOS_CMAKE_INCLUDE})
  endforeach()
endif()

################################################################################
# Load Compiler Settings depending on Compiler Type
################################################################################
if( APPLE )
  include( ${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/Toolchains/Apple.cmake)
elseif( DEFINED MSVC )
  set(CMAKE_BUILD_TYPE "Release")
  include( ${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/Toolchains/MSVC.cmake)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  include( ${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/Toolchains/Clang.cmake)
else( DEFINED GCC )
  set(GCC TRUE)
  include( ${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/Toolchains/GCC.cmake )
endif()

################################################################################
# Enable Memory Monitor if required
################################################################################
if(DEFINED MEMORYMONITOR_ENABLED)
  add_definitions(-DMEMORYMONITOR_ENABLED)
endif(DEFINED MEMORYMONITOR_ENABLED)

################################################################################
# Precheck all configurations and load thirdparty if required
################################################################################
if(NOT DEFINED CCOS_BUILDLEVEL)
  set(CCOS_BUILDLEVEL 0)
endif(NOT DEFINED CCOS_BUILDLEVEL)

set(CC_CURRENT_CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR})
include( ${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/CcOSBuildConfig.cmake )
