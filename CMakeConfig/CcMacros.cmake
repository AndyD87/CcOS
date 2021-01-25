if(NOT CC_MACRO_LOADED)
  ################################################################################
  # Setup global variables
  ################################################################################
  set(CC_MACRO_DIR ${CMAKE_CURRENT_LIST_DIR})
  set(CC_MACRO_LOADED TRUE)
  set(CC_DOWNLOAD_MAXIMUM_REPEATES 3)
  set(CC_BUILDLEVEL_NOT            0)
  set(CC_BUILDLEVEL_DEPENDEND      1)
  set(CC_BUILDLEVEL_BUILD          2)
  set(CC_BUILDLEVEL_REQUIRED       3)


  SET(CC_BUILD_ARCH_X86    0)
  SET(CC_BUILD_ARCH_X64    1)
  SET(CC_BUILD_ARCH_ARM    2)
  SET(CC_BUILD_ARCH_ARM64  3)
  SET(CC_BUILD_ARCH_XTENSA 4)

  ################################################################################
  # Load Cmake modules
  ################################################################################
  # currently no required

  ################################################################################
  # Make find files available for cmake
  ################################################################################
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/Find)

  # Avoid CMAKE Warning for Qt defined variable QT_QMAKE_EXECUTABLE
  if(QT_QMAKE_EXECUTABLE)
    # do nothing just avoid warning
  endif(QT_QMAKE_EXECUTABLE)

  # Add a custom target, wich can ran cmake with same arguments as previously
  add_custom_target(RUN_CMAKE
                    COMMAND cmake ARGS .)

  set_property( TARGET RUN_CMAKE PROPERTY FOLDER "CMakePredefinedTargets")


  ################################################################################
  # Set Filters to keep FolderStructurs for IDEs like VisualStudios
  ################################################################################
  macro( CcSetFiltersByFolders )
    foreach(file ${ARGN})
      string(REPLACE ${CMAKE_CURRENT_LIST_DIR}/ "" file ${file})
      string(REPLACE "//" "/" file ${file})
      get_filename_component( DIRNAME ${file} DIRECTORY)
      if(WINDOWS)
        if(NOT "${DIRNAME}" STREQUAL "")
          string(REPLACE "/" "\\" DIRNAME ${DIRNAME})
        endif()
      endif()
      if( "${DIRNAME}" STREQUAL "src")
        source_group( "" FILES ${file})
      else()
        source_group( "${DIRNAME}" FILES ${file})
      endif()
    endforeach()
  endmacro()

  ################################################################################
  # Append flags to linker flags in all build types and project types
  ################################################################################
  macro( CcAppendLinkerFlags Flags )
    set (
          LinkerFlags
            CMAKE_EXE_LINKER_FLAGS
            CMAKE_SHARED_LINKER_FLAGS
            CMAKE_STATIC_LINKER_FLAGS
            CMAKE_MODULE_LINKER_FLAGS
        )
    foreach(LinkerFlag ${LinkerFlags})
      CcAppendStringNotTwice(${LinkerFlag} ${Flags})
    endforeach()
  endmacro()

  ################################################################################
  # Append flags to static linker
  ################################################################################
  macro( CcAppendSharedLinkerFlags Flags )
    set ( LinkerFlags
            CMAKE_SHARED_LINKER_FLAGS
            CMAKE_SHARED_LINKER_FLAGS_DEBUG
            CMAKE_SHARED_LINKER_FLAGS_RELEASE
            CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO
            CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL
        )
    foreach(LinkerFlag ${LinkerFlags})
      CcAppendStringNotTwice(${LinkerFlag} ${Flags})
    endforeach()
  endmacro()

  ################################################################################
  # Set all assembler flags
  ################################################################################
  macro( CcSetAllAssemblerFlags Flags )
    set ( AssemblerFlags
            CMAKE_ASM_FLAGS
            CMAKE_ASM_FLAGS_DEBUG
            CMAKE_ASM_FLAGS_RELEASE
            CMAKE_ASM_FLAGS_RELWITHDEBINFO
            CMAKE_ASM_FLAGS_MINSIZEREL
        )
    foreach(LinkerFlag ${AssemblerFlags})
      if(NOT "${ARGN}" STREQUAL "")
        set(${LinkerFlag} ${Flags} CACHE INTERNAL "")
      else()
        set(${LinkerFlag} ${Flags})
      endif()
    endforeach()
  endmacro()

  ################################################################################
  # Append flags to static linker
  ################################################################################
  macro( CcAppendStaticLinkerFlags Flags )
    set ( LinkerFlags
            CMAKE_STATIC_LINKER_FLAGS
        )
    foreach(LinkerFlag ${LinkerFlags})
      CcAppendStringNotTwice(${LinkerFlag} ${Flags})
    endforeach()
  endmacro()

  ################################################################################
  # Append flags to linker for extutables builds
  ################################################################################
  macro( CcAppendExeLinkerFlags Flags )
    set ( LinkerFlags
            CMAKE_EXE_LINKER_FLAGS
        )
    foreach(LinkerFlag ${LinkerFlags})
      CcAppendStringNotTwice(${LinkerFlag} ${Flags})
    endforeach()
  endmacro()

  ################################################################################
  # Append flags to linker flags in all build types
  ################################################################################
  macro( CcAppendCCompilerFlags Flags)
    set ( CompilerFlags
            CMAKE_C_FLAGS
        )
    foreach(CompilerFlag ${CompilerFlags})
      CcAppendStringNotTwice(${CompilerFlag} ${Flags})
    endforeach()
  endmacro()
  ################################################################################
  # Append flags to linker flags in all build types
  ################################################################################
  macro( CcAppendCxxCompilerFlags Flags)
    set ( CompilerFlags
            CMAKE_CXX_FLAGS
        )
    foreach(CompilerFlag ${CompilerFlags})
      CcAppendStringNotTwice(${CompilerFlag} ${Flags})
    endforeach()
  endmacro()


  ################################################################################
  # Append flags to linker flags in all build types
  ################################################################################
  macro( CcAppendCompilerFlags Flags)
    CcAppendCCompilerFlags(${Flags})
    CcAppendCxxCompilerFlags(${Flags})
  endmacro()

  ################################################################################
  # Set flags to linker flags in all build types
  ################################################################################
  macro( CcSetCCompilerFlags Flags)
    set ( CompilerFlags
            CMAKE_C_FLAGS
        )
    foreach(CompilerFlag ${CompilerFlags})
      set(${CompilerFlag} ${Flags})
    endforeach()
  endmacro()
  ################################################################################
  # Set flags to linker flags in all build types
  ################################################################################
  macro( CcSetCxxCompilerFlags Flags)
    set ( CompilerFlags
            CMAKE_CXX_FLAGS
        )
    foreach(CompilerFlag ${CompilerFlags})
      set(${CompilerFlag} ${Flags})
    endforeach()
  endmacro()

  ################################################################################
  # Remove c++ compiler flags
  ################################################################################
  macro( CcRemoveCxxCompilerFlags Flags)
    set ( CompilerFlags
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_MINSIZEREL
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS_MINSIZEREL
        )
    foreach(CompilerFlag ${CompilerFlags})
      string(REPLACE "${Flags}" "" ${CompilerFlag} ${${CompilerFlag}})
    endforeach()
  endmacro()


  ################################################################################
  # Set flags to linker flags in all build types
  ################################################################################
  macro( CcSetCompilerFlags Flags)
    CcSetCCompilerFlags(${Flags})
    CcSetCxxCompilerFlags(${Flags})
  endmacro()

  ################################################################################
  # Load GuiSettings for Windows Gui Applications
  ################################################################################
  macro( CcLoadMakeProgram )
    if(CMAKE_GENERATOR)
      if(${CMAKE_GENERATOR} MATCHES "Ninja")
        message("- Load make program: ninja")
        include(${CC_MACRO_DIR}/Toolchains/ninja/Toolchain.cmake)
      elseif(${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
        message("- Load make program: make")
        include(${CC_MACRO_DIR}/Toolchains/make/Toolchain.cmake)
      endif()
    else()
      message("- Load default make program: ninja")
      include(${CC_MACRO_DIR}/Toolchains/ninja/Toolchain.cmake)
    endif()
  endmacro( CcLoadMakeProgram )

  ################################################################################
  # Get Standard Postfix for Visual Studio extension
  #   Format is msvc$MsvcVersion-$Architecture[_static][_debug][_MT]
  ################################################################################
  macro( CcVisualStudioPostFix OutputString DebugRelease StaticShared StaticSharedRuntime)
    set(VSEXTIONSION_STRING "")

    set(StaticSharedLocal "${StaticShared}")

    string(TOLOWER ${DebugRelease} DebugReleaseLower)

    if(MSVC_VERSION)
      # limit higher versions to highest known today
      if(MSVC_VERSION GREATER 1919)
        set( VSEXTIONSION_STRING "msvc1920")
      elseif(MSVC_VERSION GREATER 1910)
        set( VSEXTIONSION_STRING "msvc1910")
      else()
        set( VSEXTIONSION_STRING "msvc${MSVC_VERSION}")
      endif()
    elseif(GCC)
      message("- Visual studio version for GCC/MINGW set to 2017 and shared")
      set( VSEXTIONSION_STRING "msvc1910")
      set( StaticSharedLocal "SHARED")
    else()
      message(WARNING "- Correct visual studio version not found, use 2017")
      set( VSEXTIONSION_STRING "msvc1900")
      set( StaticSharedLocal "SHARED")
    endif()

    if("${CC_BUILD_ARCH}" STREQUAL "x64")
      set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_x64")
    elseif("${CC_BUILD_ARCH}" STREQUAL "x86")
      set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_x86")
    else()
      message(FATAL_ERROR "Unknown Architecture")
    endif()

    if("${StaticSharedLocal}" STREQUAL "STATIC")
      set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_static")
    else()
      set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_shared")
    endif()

    if("${DebugReleaseLower}" STREQUAL "debug")
      set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_debug")
    elseif("${DebugReleaseLower}" STREQUAL "minsizerel")
      set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_minsizerel")
    elseif("${DebugReleaseLower}" STREQUAL "relwithdebinfo")
      set( VSEXTIONSION_STRING "${VSEXTIONSION_STRING}_relwithdebinfo")
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
  # Print hex size for target output file and if GCC_SIZE is set
  ################################################################################
  macro( CcPrintHexSize Project)
    if(GCC_SIZE)
      add_custom_command(TARGET ${Project} POST_BUILD
              COMMENT "Invoking: Cross ARM GNU Print Size"
              COMMAND ${CMAKE_COMMAND} -E echo ""
              COMMAND ${CMAKE_COMMAND} -E echo "Size of target hex file: "
              COMMAND ${CMAKE_COMMAND} -E echo ""
              COMMAND ${GCC_SIZE} --format=berkeley --totals "$<TARGET_FILE:${Project}>"
              COMMAND ${CMAKE_COMMAND} -E echo "")
    endif(GCC_SIZE)
  endmacro()

  ################################################################################
  # Copy file from src to target, only if differs
  ################################################################################
  macro( CcCopyFile CCCOPYFILE_SRC CCCOPYFILE_TARGET)
    if(EXISTS ${CCCOPYFILE_TARGET})
      file(READ ${CCCOPYFILE_SRC} CCCOPYFILE_SRC_VAR)
      file(READ ${CCCOPYFILE_TARGET} CCCOPYFILE_TARGET_VAR)
      if("${CCCOPYFILE_SRC_VAR}" STREQUAL "${CCCOPYFILE_TARGET_VAR}")
      else("${CCCOPYFILE_SRC_VAR}" STREQUAL "${CCCOPYFILE_TARGET_VAR}")
        execute_process(  COMMAND ${CMAKE_COMMAND} -E copy ${CCCOPYFILE_SRC} ${CCCOPYFILE_TARGET})
      endif("${CCCOPYFILE_SRC_VAR}" STREQUAL "${CCCOPYFILE_TARGET_VAR}")
    else(EXISTS ${CCCOPYFILE_TARGET})
      execute_process(  COMMAND ${CMAKE_COMMAND} -E copy ${CCCOPYFILE_SRC} ${CCCOPYFILE_TARGET})
    endif(EXISTS ${CCCOPYFILE_TARGET})
  endmacro()

  ################################################################################
  # Move file from src to target, only if differs
  ################################################################################
  macro( CcMoveFile CCCOPYFILE_SRC CCCOPYFILE_TARGET)
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
    set(WIX_CACHE_DIR       "${CC_CACHE_DIR}/Tools/wix-portable")
    set(WIX_VERSION         3.11)
    CcDownloadAndExtract( "wix-portable"
                          "${WIX_CACHE_DIR}"
                          "https://coolcow.de/projects/ThirdParty/WiXToolset/binaries/${WIX_VERSION}/wix-portable.7z")
    set(CPACK_WIX_ROOT "${WIX_CACHE_DIR}")
  endmacro( CcLoadWixTools)

  ################################################################################
  # Print all available Variables wich have an specified prefix
  ################################################################################
  macro(PrintVariablesWithPrefix _prefix )
    get_cmake_property(_vars VARIABLES)
    string (REGEX MATCHALL "(^|;)${_prefix}[A-Za-z0-9_]*" _matchedVars "${_vars}")
    foreach(var ${_matchedVars})
      message("${var} : ${${var}}")
    endforeach(var ${_vars})
  endmacro()

  ################################################################################
  # Add Test of specied project and keep release/debug folders if required
  ################################################################################
  macro(CcAddTest Project )
    # Get number of configurations to build
    list(LENGTH CMAKE_CONFIGURATION_TYPES CMAKE_CONFIGURATION_TYPES_COUNT)
    if(CMAKE_CONFIGURATION_TYPES_COUNT GREATER 0)
      # Setup a test for each configuration
      foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
        string(TOUPPER ${OUTPUTCONFIG} UPPER_TYPE)
        add_test( NAME    ${Project}_${OUTPUTCONFIG}
                  CONFIGURATIONS ${OUTPUTCONFIG}
                  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${UPPER_TYPE}}/${Project}
                  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${UPPER_TYPE}} )
      endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
    else()
      # No configuration was found, default build settings are used
      add_test( NAME    ${Project}
                CONFIGURATIONS
                COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${Project}
                WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
    endif()
    CcPrintHexSize(${Project})
  endmacro()

  ################################################################################
  # Add a test directory to current project, it will check if dir existst and
  # if apps are allowed
  ################################################################################
  macro(CcAddTestDir Directory )
    if(NOT CCOS_NO_APPS)
      if( IS_DIRECTORY ${Directory})
        add_subdirectory("test")
      endif()
    endif()
  endmacro()

  ################################################################################
  # Append a string to a variable only if it is not existing
  ################################################################################
  macro(CcAppendStringNotTwice Target StringToAdd )
    set(_TestString "${StringToAdd}")
    string(REPLACE "+" "\\+" _TestString ${_TestString})
    string(REPLACE "(" "\\)" _TestString ${_TestString})
    string(REPLACE "." "\\." _TestString ${_TestString})
    string(REPLACE ")" "\\)" _TestString ${_TestString})
    string(REPLACE "[" "\\[" _TestString ${_TestString})
    string(REPLACE "]" "\\]" _TestString ${_TestString})
    string(REPLACE "*" "\\*" _TestString ${_TestString})
    if(${Target} MATCHES ${_TestString})
      # do not set twice
    else()
      set(${Target} "${${Target}} ${StringToAdd}")
    endif()
  endmacro(CcAppendStringNotTwice)

  ################################################################################
  # Remove a string from variable
  ################################################################################
  macro(CcRemoveString Target StringToRemove )
    string(REPLACE "${StringToRemove}" "" ${Target} "${${Target}}")
  endmacro(CcRemoveString)

  ################################################################################
  # Download an archive and extract
  # If this direcotry exists, the download will be skipped.
  # If an error or interruption occured, the download will repeated next time.
  # @param TargetName: Name of Package, just for output to commandline
  # @param TargetDir:  Target output directory to extract packte to.
  #                    This path will also be used for temporary files:
  #                       ${TargetDir}.zipped
  #                       ${TargetDir}.progress
  # @param SourceUrl: Url to download package from
  ################################################################################
  macro(CcDownloadAndExtract TargetName TargetDir SourceUrl )
    set(TargetZipFile "${TargetDir}.zipped")
    set(TargetProgress "${TargetDir}.progress")
    if(EXISTS ${TargetProgress})
      if(EXISTS ${TargetZipFile})
        file(REMOVE ${TargetZipFile})
      endif()
      if(EXISTS ${TargetDir})
        file(REMOVE_RECURSE ${TargetDir})
      endif()
    elseif(EXISTS ${TargetDir})
      # Do not create progress!
    else()
      file(WRITE ${TargetProgress} "In progress")
    endif()
    if(NOT EXISTS ${TargetDir})
      if(NOT EXISTS ${TargetZipFile})
        message("- Download ${TargetName}")
        file( DOWNLOAD ${SourceUrl}
              ${TargetZipFile}
              STATUS DOWNLOAD_STATUS)
        list(GET DOWNLOAD_STATUS 0 NUMERIC_STATUS)
        if(NOT ${NUMERIC_STATUS} EQUAL 0)
          file(REMOVE ${TargetZipFile})
          message(FATAL_ERROR "- Download result: ${DOWNLOAD_STATUS}")
        else()
          message("- Download succeeded, extracting")
        endif()
      endif()

      if(EXISTS ${TargetZipFile})
        file(MAKE_DIRECTORY ${TargetDir})
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${TargetZipFile}
                        WORKING_DIRECTORY                  ${TargetDir}
                        RESULT_VARIABLE TargetZipFile_EXTRACT_RESULT)
        if(${TargetZipFile_EXTRACT_RESULT} EQUAL 0)
          message("- Extracting succeeded")
          file(REMOVE ${TargetZipFile})
          if(EXISTS ${TargetProgress})
            file(REMOVE ${TargetProgress})
          endif()
        else()
          file(REMOVE ${TargetZipFile})
          file(REMOVE_RECURSE ${TargetDir})
          message(FATAL_ERROR "- Extract error: ${TargetZipFile_EXTRACT_RESULT}")
        endif()
      endif()
    else(NOT EXISTS ${TargetDir})
      message("- Download ${TargetName} not required: ${TargetDir}")
    endif()
  endmacro()

  ################################################################################
  # Download an archive and extract
  # If this direcotry exists, the download will be skipped.
  # If an error or interruption occured, the download will repeated next time.
  # @param Url: Url to clone from
  # @param TargetDir:  Target output directory to extract packte to.
  #                    This path will also be used for temporary files:
  #                       ${TargetDir}.zipped
  #                       ${TargetDir}.progress
  # @param SourceUrl: Url to download package from
  ################################################################################
  macro(CcGitClone TargetDir Url)
    set(CURRENT_URL ${Url})
    set(TargetProgress "${TargetDir}.progress")
    if(EXISTS ${TargetProgress})
      if(EXISTS ${TargetZipFile})
        file(REMOVE ${TargetZipFile})
      endif()
      if(EXISTS ${TargetDir})
        file(REMOVE_RECURSE ${TargetDir})
      endif()
    elseif(EXISTS ${TargetDir})
      # Do not create progress!
    else()
      file(WRITE ${TargetProgress} "In progress")
    endif()
    if(NOT EXISTS ${TargetDir})
      message("- Cloning from ${CURRENT_URL}")
      set(CCSUCCESS FALSE)
      set(CCCOUNT 0)
      set(CCARGCOUNT 2)
      while(${CCCOUNT} LESS ${CC_DOWNLOAD_MAXIMUM_REPEATES} AND
            ${CCSUCCESS} STREQUAL FALSE
      )
        execute_process(COMMAND git clone "${CURRENT_URL}" "${TargetDir}"
                        RESULT_VARIABLE Clone_EXTRACT_RESULT
                        OUTPUT_QUIET ERROR_QUIET
                        )
        if(${Clone_EXTRACT_RESULT} EQUAL 0)
          set(CCSUCCESS TRUE)
          if(EXISTS ${TargetProgress})
            file(REMOVE ${TargetProgress})
          endif()
          message("- Cloning succeeded")
          if(ARGN)
            execute_process(COMMAND git checkout "${ARG0}"
                            RESULT_VARIABLE Clone_EXTRACT_RESULT
                            OUTPUT_QUIET ERROR_QUIET
                            WORKING_DIRECTORY
                            )
            if(${Clone_EXTRACT_RESULT} EQUAL 0)
              message("- Checkout succeeded")
            else()
              message("- Checkout failed working with master")
            endif()
          endif()
        else()
          MATH(EXPR CCCOUNT "${CCCOUNT}+1")
          if(${CCCOUNT} LESS ${CC_DOWNLOAD_MAXIMUM_REPEATES})
            message("- Cloning failed, retry")
          elseif(CCARGCOUNT LESS ${ARGC})
            set(list_var "${ARGN}")
            list(GET list_var 0 CURRENT_URL)
            message("- Cloning failed, next mirror nr: ${CURRENT_URL}")
            set(CCCOUNT 0)
            MATH(EXPR CCARGCOUNT "${CCARGCOUNT}+1")
          else()
            message(FATAL_ERROR "- Cloning failed: ${Clone_EXTRACT_RESULT}")
            if(EXISTS ${TargetProgress})
              file(REMOVE ${TargetProgress})
            endif()
          endif()
        endif()
      endwhile()
    else()
      message("- Cloning not required from ${CURRENT_URL}")
    endif()
  endmacro()

  macro(CcGitUpdate Repository)
    execute_process(COMMAND git pull
                    WORKING_DIRECTORY ${Repository}
                    RESULT_VARIABLE Git_EXTRACT_RESULT
                    OUTPUT_QUIET ERROR_QUIET)
    if(NOT ${Git_EXTRACT_RESULT} EQUAL 0)
      message(FATAL_ERROR "Git pull failed (${Git_EXTRACT_RESULT}): ${Repository}")
    endif()
  endmacro(CcGitUpdate Repository)

  macro(CcGitCheckout Repository Commit)
    execute_process(COMMAND git checkout ${Commit}
                    WORKING_DIRECTORY ${Repository}
                    RESULT_VARIABLE Git_EXTRACT_RESULT
                    #OUTPUT_QUIET ERROR_QUIET
    )
    if(NOT ${Git_EXTRACT_RESULT} EQUAL 0)
      message(FATAL_ERROR "Git checkout failed (${Git_EXTRACT_RESULT}): ${Commit} ${Repository}")
    endif()
  endmacro()

  macro(CcGitPatchApply Repository Patch)
    execute_process(COMMAND git apply ${Patch}
                    WORKING_DIRECTORY ${Repository}
                    RESULT_VARIABLE Git_EXTRACT_RESULT
                    #OUTPUT_QUIET ERROR_QUIET
    )
    if(NOT ${Git_EXTRACT_RESULT} EQUAL 0)
      message(FATAL_ERROR "Git patch failed (${Git_EXTRACT_RESULT}): ${Patch} ${Repository}")
    endif()
  endmacro(CcGitPatchApply)

  macro(CcGitUpdateAndCheckout Repository Commit)
      CcGitUpdate(${Repository})
      CcGitCheckout(${Repository} ${Commit})
  endmacro(CcGitUpdateAndCheckout Repository Commit)

  ################################################################################
  # Append a variable to list only if not existing
  ################################################################################
  macro(CcListAppendOnce Target StringToAdd )
    list (FIND ${Target} "${StringToAdd}" _index)
    if (${_index} EQUAL -1)
      list(APPEND ${Target} "${StringToAdd}")
    endif()
  endmacro()

  ################################################################################
  # Add library with macro to this project
  ################################################################################
  macro(CcAddLibrary ProjectName LinkType Sources)
      set(AddLibrary_SOURCES ${Sources})
      foreach (_src ${ARGN})
        CcListAppendOnce(AddLibrary_SOURCES "${_src}")
      endforeach()
      if(COMMAND CcAddLibraryOverride)
        CcAddLibraryOverride(${ProjectName} ${LinkType} ${AddLibrary_SOURCES})
      else()
        add_library(${ProjectName} ${LinkType} ${AddLibrary_SOURCES})
        if(MINGW)
          set_target_properties(${ProjectName} PROPERTIES PREFIX "")
        endif()
      endif()
  endmacro()

  ################################################################################
  # Add driver with macro to this project
  ################################################################################
  macro(CcAddDriver ProjectName Sources)
      set(AddDriver_SOURCES ${Sources})
      foreach (_src ${ARGN})
        CcListAppendOnce(AddDriver_SOURCES "${_src}")
      endforeach()
      if(COMMAND CcAddDriverOverride)
        CcAddDriverOverride(${ProjectName} ${AddDriver_SOURCES})
      else()
        message(FATAL_ERROR "Driver requires CcAddDriverOverride")
      endif()
  endmacro()

  ################################################################################
  # Add driver library with macro to this project
  ################################################################################
  macro(CcAddDriverLibrary ProjectName Sources)
      set(AddDriver_SOURCES ${Sources})
      foreach (_src ${ARGN})
        CcListAppendOnce(AddDriver_SOURCES "${_src}")
      endforeach()
      if(COMMAND CcAddDriverLibraryOverride)
        CcAddDriverLibraryOverride(${ProjectName} ${AddDriver_SOURCES})
      else()
        message(FATAL_ERROR "Driver requires CcAddDriverLibraryOverride")
      endif()
  endmacro()

  ################################################################################
  # Add executable with makro to this project
  ################################################################################
  macro(CcAddExecutable ProjectName Sources)
      set(AddExecutable_SOURCES ${Sources})
      foreach (_src ${ARGN})
        CcListAppendOnce(AddExecutable_SOURCES "${_src}")
      endforeach()
      if(COMMAND CcAddExecutableOverride)
        CcAddExecutableOverride(${ProjectName} ${AddExecutable_SOURCES})
      elseif(CC_APPLICATION_LIB)
	      if(NOT "${ProjectName}" STREQUAL "")
          CcAddLibrary(${ProjectName} STATIC ${AddExecutable_SOURCES})
	      endif()
        CcAddLibrary(${CC_APPLICATION_LIB} STATIC "${CCOS_DIR}/Resources/dummy.c")
        target_link_libraries(${CC_APPLICATION_LIB} ${ProjectName})
      else()
        add_executable(${ProjectName} ${AddExecutable_SOURCES})
    	CcPrintHexSize(${ProjectName})
      endif()
  endmacro()

  ################################################################################
  # Add gui executable like CcAddExecutable
  ################################################################################
  macro(CcAddGuiExecutable ProjectName Sources)
    set(AddExecutable_SOURCES ${Sources})
    foreach (_src ${ARGN})
      CcListAppendOnce(AddExecutable_SOURCES "${_src}")
    endforeach()
    if(DEFINED MSVC)
      set ( LinkerFlags
              CMAKE_EXE_LINKER_FLAGS_DEBUG
              CMAKE_EXE_LINKER_FLAGS_RELEASE
              CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
              CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
          )
      foreach(LinkerFlag ${LinkerFlags})
        # For Windows set Subsystem to Windows (/SUBSYSTEM:CONSOLE was set before)
        # keep entry point on main(argc, argv)
        string(REPLACE "/SUBSYSTEM:CONSOLE" "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup" ${LinkerFlag} "${${LinkerFlag}}")
        # afxcwd.lib libcpmtd.lib must be set in right order
        if("${LinkerFlag} " MATCHES "DEBUG ")
          set(${LinkerFlag} "${${LinkerFlag}} uafxcwd.lib libcpmtd.lib")
        else()
          set(${LinkerFlag} "${${LinkerFlag}} uafxcw.lib libcpmt.lib")
        endif()
      endforeach()
    elseif(DEFINED GCC AND DEFINED WINDOWS)
      set ( LinkerFlags
              CMAKE_EXE_LINKER_FLAGS_DEBUG
              CMAKE_EXE_LINKER_FLAGS_RELEASE
              CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
              CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
          )
      foreach(LinkerFlag ${LinkerFlags})
        set(${LinkerFlag} "${${LinkerFlag}} -Wl,-subsystem,windows")
      endforeach()
    endif(DEFINED MSVC)
    CcAddExecutable(${ProjectName} ${AddExecutable_SOURCES})
  endmacro()

  ################################################################################
  # Add string resource to project
  ################################################################################
  macro(CcAddResourceString ResourceDir ResourceFileName VariableName FileList)
    add_custom_command( OUTPUT ${ResourceDir}/${ResourceFileName}.c
                               ${ResourceDir}/${ResourceFileName}.h
                        COMMAND CcOSResource -i ${ResourceFileName} -o ${ResourceFileName} -n ${VariableName} -s
                        DEPENDS ${ResourceDir}/${ResourceFileName}
                        WORKING_DIRECTORY ${ResourceDir}
    )
    CcListAppendOnce( ${FileList} ${ResourceDir}/${ResourceFileName}.c)
    CcListAppendOnce( ${FileList} ${ResourceDir}/${ResourceFileName}.h)
    if(NOT EXISTS ${ResourceDir}/${ResourceFileName}.c)
      message("-- create Resource ${ResourceFileName}")
      execute_process(COMMAND CcOSResource -i ${ResourceFileName} -o ${ResourceFileName} -n ${VariableName} -s
                      WORKING_DIRECTORY ${ResourceDir})
    elseif(NOT EXISTS ${ResourceDir}/${ResourceFileName}.h)
      message("-- create Resource ${ResourceFileName}")
      execute_process(COMMAND CcOSResource -i ${ResourceFileName} -o ${ResourceFileName} -n ${VariableName} -s
                      WORKING_DIRECTORY ${ResourceDir})
    endif(NOT EXISTS ${ResourceDir}/${ResourceFileName}.c)
  endmacro()

  ################################################################################
  # Add binary resource to project
  ################################################################################
  macro(CcAddResourceBinary ResourceDir ResourceFileName VariableName FileList)
    add_custom_command( OUTPUT ${ResourceDir}/${ResourceFileName}.c
                               ${ResourceDir}/${ResourceFileName}.h
                        COMMAND CcOSResource -i ${ResourceFileName} -o ${ResourceFileName} -n ${VariableName}
                        DEPENDS ${ResourceDir}/${ResourceFileName}
                        WORKING_DIRECTORY ${ResourceDir}
    )
    CcListAppendOnce( ${FileList} ${ResourceDir}/${ResourceFileName}.c)
    CcListAppendOnce( ${FileList} ${ResourceDir}/${ResourceFileName}.h)
    if(NOT EXISTS ${ResourceDir}/${ResourceFileName}.c)
      message("-- create Resource ${ResourceFileName}")
      execute_process(COMMAND CcOSResource -i ${ResourceFileName} -o ${ResourceFileName} -n ${VariableName}
                      WORKING_DIRECTORY ${ResourceDir})
    elseif(NOT EXISTS ${ResourceDir}/${ResourceFileName}.h)
      message("-- create Resource ${ResourceFileName}")
      execute_process(COMMAND CcOSResource -i ${ResourceFileName} -o ${ResourceFileName} -n ${VariableName}
                      WORKING_DIRECTORY ${ResourceDir})
    endif(NOT EXISTS ${ResourceDir}/${ResourceFileName}.c)
  endmacro()

  ################################################################################
  # Check if a variable was already defined, if not, take new value.
  # Very usefull to check for variables wich can be modified from outside.
  ################################################################################
  macro(CcSetIfNotDefined VAR VALUE)
    if(NOT DEFINED ${VAR})
      set(${VAR} ${VALUE})
    endif(NOT DEFINED ${VAR})
  endmacro()
  
  ################################################################################
  # Setup Version Variables by Version Number
  ################################################################################
  macro(CcGenerateVersion VarName Major Minor Patch Build)
    set( ${VarName}_MAJOR   ${Major}  )
    set( ${VarName}_MINOR   ${Minor}  )
    set( ${VarName}_PATCH   ${Patch}  )
    set( ${VarName}_BUILD   ${Build}  )
    set( ${VarName}_VERSION_DOT   ${Major}.${Minor}.${Patch}.${Build}  )
    set( ${VarName}_VERSION_COLON ${Major},${Minor},${Patch},${Build}  )
  endmacro(CcGenerateVersion)
  
  ################################################################################
  # Update Output binary path in all configurations and append relative path.
  ################################################################################
  macro(CcAppendToBinaryOuptutPath PathToAppend)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PathToAppend}")
    foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
      string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}}/${PathToAppend}")
    endforeach()
  endmacro(CcAppendToBinaryOuptutPath)

  ################################################################################
  # Generate Version-File for applications or libs
  # usage: CcGenerateVersionFile( ProjectName [SourceFiles] )
  ################################################################################
  macro( CcGenerateVersionFile VersionFileName CmakeVersionPrefix)
    set(VersionFileName ${VersionFileName})
    set(Version_MAJOR ${${CmakeVersionPrefix}_MAJOR})
    set(Version_MINOR ${${CmakeVersionPrefix}_MINOR})
    set(Version_PATCH ${${CmakeVersionPrefix}_PATCH})
    set(Version_BUILD ${${CmakeVersionPrefix}_BUILD})
    message("-- Generate VersionFile: ${VersionFileName} with ${Version_MAJOR}.${Version_MINOR}.${Version_PATCH}.${Version_BUILD}")
    configure_file( ${CC_MACRO_DIR}/InputFiles/Version.h.in ${CMAKE_CURRENT_BINARY_DIR}/${VersionFileName}.h.tmp @ONLY IMMEDIATE)
    CcCopyFile(${CMAKE_CURRENT_BINARY_DIR}/${VersionFileName}.h.tmp ${CMAKE_CURRENT_BINARY_DIR}/${VersionFileName}.h)
    
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    if(DEFINED SOURCE_FILES)
      list(APPEND SOURCE_FILES "${CMAKE_CURRENT_BINARY_DIR}/${VersionFileName}.h")
    endif()
  endmacro()
endif(NOT CC_MACRO_LOADED)
