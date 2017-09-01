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
# Get Standard Postfix for Visual Studio extension
#   Format is $VisualStudioYear-$Architecture[_static][_debug][_MT]
################################################################################
MACRO( CcVisualStudioPostFix OutputString DebugRelease StaticShared StaticSharedRuntime)
  SET(VSEXTIONSION_STRING "")
  if(MSVC_VERSION)
    if( ${MSVC_VERSION} STREQUAL "1800" )
      SET( VSEXTIONSION_STRING "2013")
    elseif( ${MSVC_VERSION} STREQUAL "1900" )
      SET( VSEXTIONSION_STRING "2015")
    elseif( ${MSVC_VERSION} STREQUAL "1910" )
      SET( VSEXTIONSION_STRING "2017")
    else()
      # Correct visual studio version not found, use latest Version
      SET( VSEXTIONSION_STRING "2017")
    endif()
  else()
    # Maybe an other compiler is in use, select latest Version  
    SET( VSEXTIONSION_STRING "2017")
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
# Get a List of Subdirectories
################################################################################
MACRO(CcGetSubDirs SubDirs CurrentDir)
  FILE(GLOB DirItems RELATIVE ${CurrentDir} ${CurrentDir}/*)
  SET(LocalDirList "")
  FOREACH(DirItem ${DirItems})
    IF(IS_DIRECTORY ${CurrentDir}/${DirItem})
      LIST(APPEND LocalDirList ${DirItem})
    ENDIF()
  ENDFOREACH()
  SET(${SubDirs} ${LocalDirList})
ENDMACRO()

################################################################################
# Rename Endings of Project output file to CcOS default.
# CURRENTLY NOT IN USE!!
################################################################################
MACRO( CcPrintAllVars )
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
      message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
ENDMACRO()