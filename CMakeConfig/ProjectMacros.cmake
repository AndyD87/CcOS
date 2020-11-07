set(CCOS_CMAKECONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig)

################################################################################
# Setup default installation targets for a project
################################################################################
macro(CcOSSetInstall ProjectName )
  install( TARGETS  ${ProjectName}
           EXPORT  "${ProjectName}Config"
           RUNTIME DESTINATION bin
           LIBRARY DESTINATION lib
           ARCHIVE DESTINATION lib/static
           OBJECTS DESTINATION lib/static
           PUBLIC_HEADER DESTINATION include/${ProjectName}
         )
endmacro()

################################################################################
# Add Xml Configurations to cmake install
################################################################################
macro (CcOSSetInstallConfig ProjectName )
  install( DIRECTORY    ${CMAKE_CURRENT_SOURCE_DIR}/config/
           DESTINATION  config/${ProjectName}
           PATTERN "*.xml" )
endmacro()

################################################################################
# Set Cmake Version Info to Project
################################################################################
macro (CcSetOSVersion ProjectName )
  set_target_properties(  ${ProjectName}
                          PROPERTIES
                          VERSION ${CCOS_VERSION_CMAKE}
                          SOVERSION ${CCOS_VERSION_CMAKE})
endmacro()

################################################################################
# Setup Include Directorys for importing CcOS
################################################################################
macro( CcOSTargetIncludeDirs ProjectName )
  foreach(DIR ${ARGN})
    list(APPEND DIRS ${DIR} )
    target_include_directories(${ProjectName} PUBLIC $<BUILD_INTERFACE:${DIR}> )
  endforeach()
  target_include_directories( ${ProjectName} PUBLIC
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/${ProjectName}> )
endmacro()

################################################################################
# Generate RC-File for MSVC Builds, output is a Version.h File in current dir
# usage: CcOSGenerateRcFileToCurrentDir( ProjectName [SourceFiles] )
################################################################################
macro( CcOSGenerateRcFileToCurrentDir ProjectName )
  set(PROJECT_NAME "${ProjectName}")
  configure_file( ${CCOS_CMAKECONFIG_DIR}/InputFiles/ProjectVersion.rc.in ${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}_Version.rc.tmp @ONLY)
  CcMoveFile(${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}_Version.rc.tmp ${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}_Version.rc)
  if(${ARGC} GREATER 1)
    if(NOT DEFINED GCC)
      list(APPEND ${ARGV1} "${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}_Version.rc")
    endif()
  endif(${ARGC} GREATER 1)
endmacro()

################################################################################
# Rename Endings of Project output file to CcOS default.
# CURRENTLY NOT IN USE!!
################################################################################
macro( CcOSProjectNaming ProjectName )
  set_target_properties(${ProjectName} PROPERTIES OUTPUT_NAME "${ProjectName}" )
  # Debug becomes and d at the end.
  set_target_properties(${ProjectName} PROPERTIES OUTPUT_NAME_DEBUG "${ProjectName}d" )
endmacro()

################################################################################
# Set Version info for library.
# If Linux, set SOVERSION too.
################################################################################
macro( CcOSLibVersion ProjectName )
  set_target_properties(${ProjectName} PROPERTIES 
                                        VERSION ${CCOS_VERSION_CMAKE})
  if(LINUX)
    set_target_properties(${ProjectName} PROPERTIES 
                                          SOVERSION ${CCOS_VERSION_CMAKE} )
  endif(LINUX)
endmacro()

################################################################################
# Post config Steps afert CcAddLibrary:
# Usage CcOSLibSettings( ProjectName [bSetupInstall] [bSetupVersion] [sSetFiltersByFolders])
################################################################################
macro( CcOSLibSettings ProjectName )
  if(${ARGC} GREATER 1)
    if(${ARGV1} STREQUAL "TRUE")
      CcOSSetInstall(${ProjectName})
    endif(${ARGV1} STREQUAL "TRUE")
  endif(${ARGC} GREATER 1)
  
  if(${ARGC} GREATER 2)
    if(${ARGV2} STREQUAL "TRUE")
      CcOSLibVersion(${ProjectName})
    endif(${ARGV2} STREQUAL "TRUE")
  endif(${ARGC} GREATER 2)
  
  if(${ARGC} GREATER 3)
    set(FILES ${ARGN})
    list(REMOVE_AT FILES 0)
    list(REMOVE_AT FILES 0)
    list(REMOVE_AT FILES 0)
    CcSetFiltersByFolders(${FILES})
  endif(${ARGC} GREATER 3)
endmacro()

################################################################################
# Post config Steps afert add_executable:
# Usage CcOSExeSettings( ProjectName [sSetFiltersByFolders])
################################################################################
macro( CcOSExeSettings ProjectName )
  if(${ARGC} GREATER 1)
    set(FILES ${ARGN})
    CcSetFiltersByFolders(${FILES})
  endif(${ARGC} GREATER 1)
endmacro()
