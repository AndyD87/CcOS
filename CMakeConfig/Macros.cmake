SET(CCOS_CMAKECONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig)

################################################################################
# Setup default installation targets for a project
################################################################################
MACRO (CcSetInstall ProjectName )
  set_property( TARGET ${ProjectName} APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}>
              )

  INSTALL( TARGETS  ${ProjectName}
           EXPORT  "${ProjectName}Config"
           RUNTIME DESTINATION bin
           LIBRARY DESTINATION lib
           ARCHIVE DESTINATION lib/static
           PUBLIC_HEADER DESTINATION include/${ProjectName}
         )
  install(EXPORT "${ProjectName}Config" DESTINATION "lib/${ProjectName}")
  
#  INSTALL( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include
#           FILES_MATCHING PATTERN "*.h"
#           PATTERN "*/src"  EXCLUDE
#           PATTERN "*/test" EXCLUDE)
ENDMACRO()

################################################################################
# Add Xml Configurations to cmake install
################################################################################
MACRO (CcSetInstallConfig ProjectName )
  INSTALL( DIRECTORY    ${CMAKE_CURRENT_SOURCE_DIR}/config/
           DESTINATION  config/${ProjectName}
           PATTERN "*.xml" )
ENDMACRO()

################################################################################
# Set Cmake Version Info to Project
################################################################################
MACRO (CcSetOSVersion ProjectName )
  SET_TARGET_PROPERTIES(  ${ProjectName}
                          PROPERTIES
                          VERSION ${CCOS_VERSION_CMAKE}
                          SOVERSION ${CCOS_VERSION_CMAKE})
ENDMACRO()

################################################################################
# Set Filters to keep FolderStructurs for IDEs like VisualStudios
################################################################################
MACRO( CcSetFiltersByFolders )
  foreach(FILE ${ARGN})
    STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" FILETEMP ${FILE})
    STRING(REPLACE "/src/" "/" FILETEMP ${FILETEMP})
    STRING(REPLACE "/" "\\" FILETEMP ${FILETEMP})
    GET_FILENAME_COMPONENT( DIRNAME ${FILETEMP} DIRECTORY)
    if( "${DIRNAME}" STREQUAL "src")
      SOURCE_GROUP( "" FILES ${FILE})
    else()
      SOURCE_GROUP( "${DIRNAME}" FILES ${FILE})
    endif()
  ENDFOREACH()
ENDMACRO()

################################################################################
# Load GuiSettings for Windows Gui Applications
################################################################################
MACRO( CcLoadGuiSettings )
  if(DEFINED MSVC)
    # For Windows set Subsystem to Windows (/SUBSYSTEM:CONSOLE was set before)
    string(REPLACE "/SUBSYSTEM:CONSOLE" "/SUBSYSTEM:WINDOWS" CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
    # keep entry point on main(argc, argv)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup ")
  endif(DEFINED MSVC)
ENDMACRO( CcLoadGuiSettings )

################################################################################
# Setup Include Directorys for importing CcOS
################################################################################
MACRO( CcTargetIncludeDirs ProjectName )
  foreach(DIR ${ARGN})
    LIST(APPEND DIRS ${DIR} )
    target_include_directories(${ProjectName} PUBLIC $<BUILD_INTERFACE:${DIR}> )
  ENDFOREACH()
  target_include_directories( ${ProjectName} PUBLIC
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/${ProjectName}> )
ENDMACRO()

################################################################################
# Generate RC-File for MSVC Builds, output is a Version.h File in current dir
################################################################################
MACRO( CcGenerateCcOSRcFileToCurrentDir ProjectName )
  SET(PROJECT_NAME "${ProjectName}")
  configure_file( ${CCOS_CMAKECONFIG_DIR}/InputFiles/CcOSVersion.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc @ONLY)
ENDMACRO()

################################################################################
# Get Standard Postfix for Visual Studio extension
#   Format is $VisualStudioYear-$Architecture[_static][_debug][_MT]
################################################################################
MACRO( CcOSVisualStudioPostFix OutputString DebugRelease StaticShared StaticSharedRuntime)
  SET(VSEXTIONSION_STRING "")
  if( ${MSVC_VERSION} STREQUAL "1800" )
    SET( VSEXTIONSION_STRING "2013")
  elseif( ${MSVC_VERSION} STREQUAL "1900" )
    SET( VSEXTIONSION_STRING "2015")
  elseif( ${MSVC_VERSION} STREQUAL "1910" )
    SET( VSEXTIONSION_STRING "2017")
  else()
    # Correct visual studio version not found, use VS 2015
    SET( VSEXTIONSION_STRING "2015")
  endif()
  
  if("${CCOS_BUILD_ARCH}" STREQUAL "x64")
    SET( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_x64")
  elseif("${CCOS_BUILD_ARCH}" STREQUAL "x86")
    SET( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_x86")
  else()
    MESSAGE(FATAL_ERROR "Unknown Architecture")
  endif()
  
  if(${StaticShared} STREQUAL "STATIC")
    SET( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_static")
  else()
    SET( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_shared")
  endif()
  
  if(${DebugRelease} STREQUAL "DEBUG")
    SET( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_debug")
  else()
    SET( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_release")
  endif()
  
  if(${StaticSharedRuntime} STREQUAL "STATIC")
    SET( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_MT")
  else()
    SET( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_MD")
  endif()
  
  SET(${OutputString} ${VSEXTIONSION_STRING})
ENDMACRO()


################################################################################
# Rename Endings of Project output file to CcOS default.
# CURRENTLY NOT IN USE!!
################################################################################
MACRO( CcOSProjectNaming ProjectName )
  set_target_properties(${ProjectName} PROPERTIES OUTPUT_NAME "${ProjectName}" )
  # Debug becomes and d at the end.
  set_target_properties(${ProjectName} PROPERTIES OUTPUT_NAME_DEBUG "${ProjectName}d" )
ENDMACRO()
