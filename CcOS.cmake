################################################################################
# Setup Globals
################################################################################
cmake_minimum_required (VERSION 3.0)

# setup project root dir
set(CCOS_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

include(${CMAKE_CURRENT_LIST_DIR}/VERSION.cmake )
include(${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/CcMacros.cmake )
include(${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/ProjectMacros.cmake )

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
# Include Config File if config is set
################################################################################
if(DEFINED CONFIGFILE)
  include( ${CONFIGFILE} )
else()
  # Use default System Config
  if("${CMAKE_SYSTEM_NAME}" STREQUAL "Generic" OR
    GENERIC
  )
    set(GENERIC TRUE)
    set(CMAKE_SYSTEM_NAME "Generic")
    message( "- Platform: Generic" )
    include( ${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/Configs/Config.Generic.cmake)
  elseif(DEFINED WIN32)
    set(WINDOWS TRUE)
    message( "- Platform: Windows" )
    include( ${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/Configs/Config.Windows.cmake)
  else()
    set(LINUX TRUE)
    message( "- Platform: Linux" )
    include( ${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/Configs/Config.Linux.cmake)
  endif()
endif()

################################################################################
# Setup Cache directory if not yet defined
################################################################################
if(NOT DEFINED CC_CACHE_DIR)
  set( CC_CACHE_DIR ${CMAKE_CURRENT_LIST_DIR}/Cache)
  if(NOT EXISTS ${CC_CACHE_DIR})
    file(MAKE_DIRECTORY ${CC_CACHE_DIR})
  endif()
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
include( ${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/CcOSBuildConfig.cmake )

################################################################################
# Add CcOS to compile
# Add all CcLib Objects to build-List:
################################################################################
# Add ThirdParty first to resolve external dependencies
#include( ${CMAKE_CURRENT_LIST_DIR}/ThirdParty/CMakeLists.txt )
#add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/CcKernel/ )
#add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/CcGui/ )
#add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/CcNetwork/ )
#add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/CcMedia/ )
#add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/CcUtil/ )
#add_subdirectory( ${CMAKE_CURRENT_LIST_DIR}/CcModules/ )
