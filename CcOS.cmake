################################################################################
# Setup Globals
################################################################################
cmake_minimum_required (VERSION 2.8)
if(CMAKE_PROJECT_NAME AND
   NOT ${CMAKE_PROJECT_NAME} EQUAL "CcOS")
  include( ${CMAKE_CURRENT_SOURCE_DIR}/VERSION.cmake )
  include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/CcMacros.cmake )
  include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/ProjectMacros.cmake )
endif()

################################################################################
# Include Config File if config is set
################################################################################
if(DEFINED CONFIGFILE)
  include( ${CONFIGFILE} )
else()
  # Use default System Config
  if("${CMAKE_SYSTEM_NAME}" STREQUAL "Generic")
    message( "- Platform: Generic" )
    include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Configs/Config.Generic.cmake)
  elseif(DEFINED WIN32)
    set(WINDOWS TRUE)
    message( "- Platform: Windows" )
    include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Configs/Config.Windows.cmake)
  else()
    set(LINUX TRUE)
    message( "- Platform: Linux" )
    include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Configs/Config.Linux.cmake)
  endif()
endif()

################################################################################
# Setup Cache directory if not yet defined
################################################################################
if(NOT DEFINED CC_CACHE_DIR)
  set( CC_CACHE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Cache)
  if(NOT EXISTS ${CC_CACHE_DIR})
    file(MAKE_DIRECTORY ${CC_CACHE_DIR})
  endif()
endif()

################################################################################
# Load Compiler Settings depending on Compiler Type
################################################################################
if( APPLE )
  include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/Apple.cmake)
elseif( DEFINED MSVC )
  set(CMAKE_BUILD_TYPE "Release")
  include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/MSVC.cmake)
else( DEFINED GCC )
  include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/GCC.cmake )
endif()

################################################################################
# Precheck all configurations and load thirdparty if required
################################################################################
if(NOT DEFINED CCOS_BUILDLEVEL)
  set(CCOS_BUILDLEVEL 0)
endif(NOT DEFINED CCOS_BUILDLEVEL)
include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/CcOSBuildConfig.cmake )

################################################################################
# Enable Memory Monitor if required
################################################################################
if(DEFINED MEMORYMONITOR_ENABLED)
  add_definitions(-DMEMORYMONITOR_ENABLED)
endif(DEFINED MEMORYMONITOR_ENABLED)

################################################################################
# Add CcOS to compile
# Add all CcLib Objects to build-List:
################################################################################
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcKernel/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcGui/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcNetwork/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcMedia/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcUtil/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcModules/ )
