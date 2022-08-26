# setup project root dir
set(CCOS_DIR ${CMAKE_CURRENT_LIST_DIR}/.. CACHE INTERNAL "")
set(CCOS_CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR})

if(NOT EXISTS ${CCOS_CONFIG_DIR}/CcBuildConfig/CcBuildConfig.cmake)
  execute_process(COMMAND git submodule init "${CCOS_CONFIG_DIR}/CcBuildConfig"
          WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
  execute_process(COMMAND git submodule update "${CCOS_CONFIG_DIR}/CcBuildConfig"
          WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endif()
include(${CCOS_CONFIG_DIR}/CcBuildConfig/CcBuildConfig.cmake)

################################################################################
# Load Macros from CcOS
################################################################################
include(${CCOS_CONFIG_DIR}/ProjectMacros.cmake )

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


macro(CcOSLoadProjects)
  ################################################################################
  # Precheck all configurations and load thirdparty if required
  ################################################################################
  if(NOT DEFINED CCOS_BUILDLEVEL)
    set(CCOS_BUILDLEVEL 0)
  endif(NOT DEFINED CCOS_BUILDLEVEL)

  set(CC_CURRENT_CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR})
  include( ${CCOS_CONFIG_DIR}/CcOSBuildConfig.cmake )
endmacro()
