################################################################################
# Set Filters to keep FolderStructurs for IDEs like VisualStudios
################################################################################
macro( CcSetFiltersByFolders )
  foreach(file ${ARGN})
    string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" FILETEMP ${file})
    string(REPLACE "/src/" "/" FILETEMP ${FILETEMP})
    string(REPLACE "/" "\\" FILETEMP ${FILETEMP})
    get_filename_component( DIRNAME ${FILETEMP} DIRECTORY)
    if( "${DIRNAME}" STREQUAL "src")
      source_group( "" FILES ${file})
    else()
      source_group( "${DIRNAME}" FILES ${file})
    endif()
  endforeach()
endmacro()

################################################################################
# Load GuiSettings for Windows Gui Applications
################################################################################
macro( CcLoadGuiSettings )
  if(DEFINED MSVC)
    set ( CompilerFlags
            CMAKE_EXE_LINKER_FLAGS
            CMAKE_EXE_LINKER_FLAGS_DEBUG
            CMAKE_EXE_LINKER_FLAGS_RELEASE
            CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
            CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
        )
    foreach(CompilerFlag ${CompilerFlags})
      # For Windows set Subsystem to Windows (/SUBSYSTEM:CONSOLE was set before)
      # keep entry point on main(argc, argv)
      string(REPLACE "/SUBSYSTEM:CONSOLE" "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
  endif(DEFINED MSVC)
endmacro( CcLoadGuiSettings )

################################################################################
# Get Standard Postfix for Visual Studio extension
#   Format is $VisualStudioYear-$Architecture[_static][_debug][_MT]
################################################################################
macro( CcVisualStudioPostFix OutputString DebugRelease StaticShared StaticSharedRuntime)
  set(VSEXTIONSION_STRING "")
  if(MSVC_VERSION)
    # limit higher versions to highest known today
    if(MSVC_VERSION GREATER 1910)
      set( VSEXTIONSION_STRING "msvc1910")
    else()
      set( VSEXTIONSION_STRING "msvc${MSVC_VERSION}")
    endif()
  else()
    message(WARNING "- Correct visual studio version not found, use 2015") 
    set( VSEXTIONSION_STRING "msvc1900")
  endif()
  
  if("${CCOS_BUILD_ARCH}" STREQUAL "x64")
    set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_x64")
  elseif("${CCOS_BUILD_ARCH}" STREQUAL "x86")
    set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_x86")
  else()
    message(FATAL_ERROR "Unknown Architecture")
  endif()
  
  if("${StaticShared}" STREQUAL "STATIC")
    set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_static")
  else()
    set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_shared")
  endif()
  
  if("${DebugRelease}" STREQUAL "DEBUG")
    set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_debug")
  else()
    set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_release")
  endif()
  
  if("${StaticSharedRuntime}" STREQUAL "STATIC")
    set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_MT")
  else()
    set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_MD")
  endif()
  
  set(${OutputString} ${VSEXTIONSION_STRING})
endmacro()

################################################################################
# Get a List of Subdirectories
################################################################################
macro(CcGetSubDirs SubDirs CurrentDir)
  file(GLOB DirItems RELATIVE ${CurrentDir} ${CurrentDir}/*)
  set(LocalDirList "")
  foreach(DirItem ${DirItems})
    if(IS_DIRECTORY ${CurrentDir}/${DirItem})
      list(APPEND LocalDirList ${DirItem})
    endif()
  endforeach()
  set(${SubDirs} ${LocalDirList})
endmacro()

################################################################################
# Print all available Variables on current scope
################################################################################
macro( CcPrintAllVars )
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
      message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endmacro()

################################################################################
# Do not use autogenerated Release Debug from selected Generator
################################################################################
macro( CcNoConfigurationDirs )
  foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
      string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
      set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
      set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
      set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} )
  endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
endmacro( CcNoConfigurationDirs )


################################################################################
# Copy file from src to target, only if differs
################################################################################
macro( CcCopyFile CCCOPYFILE_SRC CCCOPYFILE_TARGET)
  if(EXISTS ${CCCOPYFILE_TARGET})
    file(READ ${CCCOPYFILE_SRC} CCCOPYFILE_SRC_VAR)
    file(READ ${CCCOPYFILE_TARGET} CCCOPYFILE_TARGET_VAR)
    if("${CCCOPYFILE_SRC_VAR}" STREQUAL "${CCCOPYFILE_TARGET_VAR}")
      execute_process(  COMMAND ${CMAKE_COMMAND} -E remove ${CCCOPYFILE_SRC})
    else("${CCCOPYFILE_SRC_VAR}" STREQUAL "${CCCOPYFILE_TARGET_VAR}")
      execute_process(  COMMAND ${CMAKE_COMMAND} -E copy ${CCCOPYFILE_SRC} ${CCCOPYFILE_TARGET})
      execute_process(  COMMAND ${CMAKE_COMMAND} -E remove ${CCCOPYFILE_SRC})
    endif("${CCCOPYFILE_SRC_VAR}" STREQUAL "${CCCOPYFILE_TARGET_VAR}")
  else(EXISTS ${CCCOPYFILE_TARGET})
    execute_process(  COMMAND ${CMAKE_COMMAND} -E copy ${CCCOPYFILE_SRC} ${CCCOPYFILE_TARGET})
    execute_process(  COMMAND ${CMAKE_COMMAND} -E remove ${CCCOPYFILE_SRC})
  endif(EXISTS ${CCCOPYFILE_TARGET})
endmacro()

################################################################################
# Setup Wix Tools for generating an MSI for windows
################################################################################
macro( CcLoadWixTools )
  # setup varibles for command
  set(WIX_CACHE_DIR       ${CCOS_CACHE_DIR}/Tools)
  set(WIX_ZIP_FOLDERNAME  "wix-portable" )
  set(WIX_ZIP_FILENAME    "wix-portable.7z" )
  set(WIX_ZIP_FILE        ${WIX_CACHE_DIR}/${WIX_ZIP_FILENAME} )
  set(WIX_ZIP_FOLDER      ${WIX_CACHE_DIR}/${WIX_ZIP_FOLDERNAME} )
  set(WIX_VERSION         3.11)
  set(DOWNLOAD_URL      "http://mirror.adirmeier.de/projects/ThirdParty/WiXToolset/binaries/${WIX_VERSION}/${WIX_ZIP_FILENAME}")

  # Extract file if not exits  
  if(NOT EXISTS ${WIX_ZIP_FOLDER})
    # Download File if required
    if( NOT EXISTS ${WIX_ZIP_FILE} )
      message("- Download WiX-Toolset: ${DOWNLOAD_URL}")
      file( DOWNLOAD 
              ${DOWNLOAD_URL} 
              ${WIX_ZIP_FILE}
            STATUS DOWNLOAD_STATUS)
      list(GET DOWNLOAD_STATUS 0 NUMERIC_STATUS)
      if(NOT ${NUMERIC_STATUS} EQUAL 0)
        file(REMOVE ${WIX_ZIP_FILE})
        message(FATAL_ERROR "- Download result: ${DOWNLOAD_STATUS}")
      else()
        message("- Download succeeded")
      endif()    
    endif()   
    file(MAKE_DIRECTORY ${WIX_ZIP_FOLDER})
    message("- Extract WiX-Toolset: ${WIX_ZIP_FILENAME}")
    execute_process(  COMMAND ${CMAKE_COMMAND} -E tar xf ${WIX_ZIP_FILE}
                      WORKING_DIRECTORY ${WIX_ZIP_FOLDER} )
    file(REMOVE ${WIX_ZIP_FILE})    
  endif()
  set(CPACK_WIX_ROOT                  ${WIX_ZIP_FOLDER})
endmacro( CcLoadWixTools)

# Avoid CMAKE Warning for Qt defined variable QT_QMAKE_EXECUTABLE
if(QT_QMAKE_EXECUTABLE)
  # do nothing just avoid warning
endif(QT_QMAKE_EXECUTABLE)

macro(PrintVariablesWithPrefix _prefix )
    get_cmake_property(_vars VARIABLES)
    string (REGEX MATCHALL "(^|;)${_prefix}[A-Za-z0-9_]*" _matchedVars "${_vars}")
    foreach(var ${_matchedVars})
      message("${var} : ${${var}}")
    endforeach(var ${_vars})
endmacro()