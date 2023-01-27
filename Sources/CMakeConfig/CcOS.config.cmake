# setup project root dir
set(CCOS_DIR ${CMAKE_CURRENT_LIST_DIR}/.. CACHE INTERNAL "")
set(CCOS_CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR})
set(THIRDPARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty)

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

macro(CcAddProject ProjectName Relpath)
  list(APPEND CC_PROJECTS ${ProjectName}
  set(CC_PROJECTS_${ProjectName}_PATH ${RelPath})
endmacro()
