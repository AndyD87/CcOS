include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Macros.cmake )

###################
# Include Config File if config is set
###################
if(DEFINED CONFIGFILE)
  include( ${CONFIGFILE} )
else()
  # Use default System Config
  if(DEFINED GENERIC)
    include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Configs/Config.Generic.cmake)
  elseif(DEFINED WIN32)
    set(WINDOWS TRUE)
    include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Configs/Config.Windows.cmake)
  elseif(DEFINED APPLE)
    include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Configs/Config.Apple.cmake)
  else()
    set(LINUX TRUE)
    include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Configs/Config.Linux.cmake)
  endif()
endif()

###################
# Setup Debug
###################
if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  message("Build-Type set to ${CMAKE_BUILD_TYPE}")
  set(CCOS_BUILD_TYPE "DEBUG" )
  set(CMAKE_VERBOSE_MAKEFILE          ON)
else()
  message("Build-Type set to release")
  set(CCOS_BUILD_TYPE "RELEASE" )
endif()

# Load Settings depending on Compiler Type
if( APPLE )
  include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/Apple.cmake)
elseif( DEFINED MSVC )
  set(CMAKE_BUILD_TYPE "Release")
  include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/MSVC.cmake)
else( DEFINED GCC )
  include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/GCC.cmake )
endif()

set(EXECUTABLE_OUTPUT_PATH            ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set(LIBRARY_OUTPUT_PATH               ${CMAKE_CURRENT_SOURCE_DIR}/bin )
link_directories( ${EXECUTABLE_OUTPUT_PATH} )
enable_testing()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

###################
# Precheck all configurations and load thirdparty if required
###################
include( ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/ConfigCheck.cmake )

###################
# Add CcOS to compile
# Add all CcLib Objects to build-List:
###################
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcKernel/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcGui/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcNetwork/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcMedia/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcUtil/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcApps/ )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/CcModules/ )
