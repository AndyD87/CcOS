################################################################################
# Setup Globals
################################################################################
if(NOT CCOS_CMAKE_CONFIG_LOADED)
set(CCOS_CMAKE_CONFIG_LOADED TRUE)

# setup project root dir
set(CCOS_DIR ${CMAKE_CURRENT_LIST_DIR}/.. CACHE INTERNAL "")
set(CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

################################################################################
# Load Macros from CcOS
################################################################################
include(${CONFIG_DIR}/CcMacros.cmake )
include(${CONFIG_DIR}/ProjectMacros.cmake )

################################################################################
# Setup Cache directory if not yet defined
################################################################################
if(NOT DEFINED CC_CACHE_DIR)
  if(DEFINED ENV{CC_CACHE_DIR})
    file(TO_CMAKE_PATH  $ENV{CC_CACHE_DIR} CC_CACHE_DIR)
  else()
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../Cache)
      set( CC_CACHE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Cache)
    else()
      set( CC_CACHE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Cache)
    endif()
  endif()
  if(NOT EXISTS ${CC_CACHE_DIR})
    file(MAKE_DIRECTORY ${CC_CACHE_DIR})
  endif()
elseif(NOT EXISTS ${CC_CACHE_DIR})
  file(MAKE_DIRECTORY ${CC_CACHE_DIR})
endif()

message("- Cache directory: ${CC_CACHE_DIR}")


################################################################################
# Setup Toolchain globaly
################################################################################
# Load Board settings if defined
if(DEFINED CCOS_BOARD)
  CcOSGetKnownBoard(${CCOS_BOARD} BoardDir)
  if(NOT ${BoardDir} STREQUAL "")
    include(${CONFIG_DIR}/${BoardDir})
  elseif(${CCOS_BOARD} MATCHES "/Config.cmake")
    include(${CONFIG_DIR}/${CCOS_BOARD})
  else()
    include(${CONFIG_DIR}/${CCOS_BOARD}/Config.cmake)
  endif()
  if(CCOS_BOARD_TYPE)
    add_definitions(-DCCOS_BOARD_TYPE=${CCOS_BOARD_TYPE})
  endif()
endif()

################################################################################
# Load Conan if available
################################################################################
if(EXISTS ${CONFIG_DIR}/Conan/conan.cmake)
  include(${CONFIG_DIR}/Conan/conan.cmake)
endif()

macro(CcOSLoadBuildSettings)
  ################################################################################
  # Setup Default Values for CMAKE
  ################################################################################
  CcNoConfigurationDirs()
  enable_testing()
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
      include( ${CONFIG_DIR}/Configs/Config.Generic.cmake)
    elseif(DEFINED WIN32)
      message( "- Platform: Windows" )
      include( ${CONFIG_DIR}/Configs/Config.Windows.cmake)
    else()
      message( "- Platform: Linux" )
      include( ${CONFIG_DIR}/Configs/Config.Linux.cmake)
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
    include( ${CONFIG_DIR}/Toolchains/Apple.cmake)
  elseif( DEFINED MSVC )
    include( ${CONFIG_DIR}/Toolchains/MSVC.cmake)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    include( ${CONFIG_DIR}/Toolchains/Clang.cmake)
  else( DEFINED GCC )
    set(GCC TRUE)
    include( ${CONFIG_DIR}/Toolchains/GCC.cmake )
  endif()

  ################################################################################
  # Enable Memory Monitor if required
  ################################################################################
  if(DEFINED MEMORYMONITOR_ENABLED)
    add_definitions(-DMEMORYMONITOR_ENABLED)
  endif(DEFINED MEMORYMONITOR_ENABLED)
endmacro()

macro(CcOSLoadProjects)
  ################################################################################
  # Precheck all configurations and load thirdparty if required
  ################################################################################
  if(NOT DEFINED CCOS_BUILDLEVEL)
    set(CCOS_BUILDLEVEL 0)
  endif(NOT DEFINED CCOS_BUILDLEVEL)

  set(CC_CURRENT_CONFIG_DIR ${CONFIG_DIR})
  include( ${CONFIG_DIR}/CcOSBuildConfig.cmake )
endmacro()

endif()
