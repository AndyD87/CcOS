################################################################################
# Setup Globals
################################################################################
if(NOT CCOS_CMAKE_CONFIG_LOADED)
set(CCOS_CMAKE_CONFIG_LOADED TRUE)

# setup project root dir
set(CCOS_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

################################################################################
# Load Macros from CcOS
################################################################################
include(${CMAKE_CURRENT_LIST_DIR}/VERSION.cmake )
include(${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/CcMacros.cmake )
include(${CMAKE_CURRENT_LIST_DIR}/CMakeConfig/ProjectMacros.cmake )

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
    include(${CMAKE_CURRENT_LIST_DIR}/${BoardDir})
  elseif(${CCOS_BOARD} MATCHES "/Config.cmake")
    include(${CMAKE_CURRENT_LIST_DIR}/${CCOS_BOARD})
  else()
    include(${CMAKE_CURRENT_LIST_DIR}/${CCOS_BOARD}/Config.cmake)
  endif()
  if(CCOS_BOARD_TYPE)
    add_definitions(-DCCOS_BOARD_TYPE=${CCOS_BOARD_TYPE})
  endif()
endif()

endif()
