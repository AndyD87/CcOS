if(NOT CC_MACRO_LOADED)
  ################################################################################
  # Setup global variables
  ################################################################################
  set(CC_MACRO_DIR ${CMAKE_CURRENT_LIST_DIR})
  set(CC_MACRO_LOADED TRUE)
  set(CC_DOWNLOAD_MAXIMUM_REPEATES 3)

  # Avoid CMAKE Warning for Qt defined variable QT_QMAKE_EXECUTABLE
  if(QT_QMAKE_EXECUTABLE)
    # do nothing just avoid warning
  endif(QT_QMAKE_EXECUTABLE)
  
  # Add a custom target, wich can ran cmake with same arguments as previously
  add_custom_target(RUN_CMAKE
                    COMMAND cmake ARGS .)


  ################################################################################
  # Set Filters to keep FolderStructurs for IDEs like VisualStudios
  ################################################################################
  macro( CcSetFiltersByFolders )
    foreach(file ${ARGN})
      string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" FILETEMP ${file})
      string(REPLACE "//" "/" FILETEMP ${FILETEMP})
      if(WINDOWS)
        string(REPLACE "\\" "/" FILETEMP ${FILETEMP})
      endif()
      get_filename_component( DIRNAME ${FILETEMP} DIRECTORY)
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
          CompilerFlags
            CMAKE_EXE_LINKER_FLAGS
            CMAKE_EXE_LINKER_FLAGS_DEBUG
            CMAKE_EXE_LINKER_FLAGS_RELEASE
            CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
            CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
            CMAKE_SHARED_LINKER_FLAGS
            CMAKE_SHARED_LINKER_FLAGS_DEBUG
            CMAKE_SHARED_LINKER_FLAGS_RELEASE
            CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO
            CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL
            CMAKE_STATIC_LINKER_FLAGS
            CMAKE_STATIC_LINKER_FLAGS_DEBUG
            CMAKE_STATIC_LINKER_FLAGS_RELEASE
            CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO
            CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL
            CMAKE_MODULE_LINKER_FLAGS
            CMAKE_MODULE_LINKER_FLAGS_DEBUG
            CMAKE_MODULE_LINKER_FLAGS_RELEASE
            CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO
            CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL
        )
    foreach(CompilerFlag ${CompilerFlags})
      set(${CompilerFlag} "${${CompilerFlag}} ${Flags} ")
    endforeach()
  endmacro()

  ################################################################################
  # Append flags to static linker
  ################################################################################
  macro( CcAppendSharedLinkerFlags Flags )
    set ( CompilerFlags
            CMAKE_SHARED_LINKER_FLAGS
            CMAKE_SHARED_LINKER_FLAGS_DEBUG
            CMAKE_SHARED_LINKER_FLAGS_RELEASE
            CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO
            CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL
        )
    foreach(CompilerFlag ${CompilerFlags})
      set(${CompilerFlag} "${${CompilerFlag}} ${Flags} ")
    endforeach()
  endmacro()

  ################################################################################
  # Append flags to static linker
  ################################################################################
  macro( CcAppendStaticLinkerFlags Flags )
    set ( CompilerFlags
            CMAKE_STATIC_LINKER_FLAGS
            CMAKE_STATIC_LINKER_FLAGS_DEBUG
            CMAKE_STATIC_LINKER_FLAGS_RELEASE
            CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO
            CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL
        )
    foreach(CompilerFlag ${CompilerFlags})
      set(${CompilerFlag} "${${CompilerFlag}} ${Flags} ")
    endforeach()
  endmacro()

  ################################################################################
  # Append flags to linker for extutables builds
  ################################################################################
  macro( CcAppendExeLinkerFlags Flags )
    set ( CompilerFlags
            CMAKE_EXE_LINKER_FLAGS
            CMAKE_EXE_LINKER_FLAGS_DEBUG
            CMAKE_EXE_LINKER_FLAGS_RELEASE
            CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
            CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
        )
    foreach(CompilerFlag ${CompilerFlags})
      set(${CompilerFlag} "${${CompilerFlag}} ${Flags} ")
    endforeach()
  endmacro()

  ################################################################################
  # Append flags to linker flags in all build types
  ################################################################################
  macro( CcAppendCompilerFlags Flags)
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
      set(${CompilerFlag} "${${CompilerFlag}} ${Flags}")
    endforeach()
  endmacro()

  ################################################################################
  # Load GuiSettings for Windows Gui Applications
  ################################################################################
  macro( CcLoadGuiSettings )
    if(DEFINED MSVC)
      set ( CompilerFlags
              CMAKE_EXE_LINKER_FLAGS_DEBUG
              CMAKE_EXE_LINKER_FLAGS_RELEASE
              CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
              CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
          )
      foreach(CompilerFlag ${CompilerFlags})
        # For Windows set Subsystem to Windows (/SUBSYSTEM:CONSOLE was set before)
        # keep entry point on main(argc, argv)
        string(REPLACE "/SUBSYSTEM:CONSOLE" "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup" ${CompilerFlag} "${${CompilerFlag}}")
        # afxcwd.lib libcpmtd.lib must be set in right order
        if("${CompilerFlag} " MATCHES "DEBUG ")
          set(${CompilerFlag} "${${CompilerFlag}} uafxcwd.lib libcpmtd.lib")
        else()
          set(${CompilerFlag} "${${CompilerFlag}} uafxcw.lib libcpmt.lib")
        endif()
      endforeach()
    elseif(DEFINED GCC AND DEFINED WINDOWS)
      set ( CompilerFlags
              CMAKE_EXE_LINKER_FLAGS_DEBUG
              CMAKE_EXE_LINKER_FLAGS_RELEASE
              CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
              CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
          )
      foreach(CompilerFlag ${CompilerFlags})
        set(${CompilerFlag} "${${CompilerFlag}} -Wl,-subsystem,windows")
      endforeach()
    endif(DEFINED MSVC)
  endmacro( CcLoadGuiSettings )

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
      if(MSVC_VERSION GREATER 1910)
        set( VSEXTIONSION_STRING "msvc1910")
      else()
        set( VSEXTIONSION_STRING "msvc${MSVC_VERSION}")
      endif()
    else()
      message(WARNING "- Correct visual studio version not found, use 2015")
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
              COMMAND echo ""
              COMMAND echo "Size of target hex file: "
              COMMAND echo ""
              COMMAND ${GCC_SIZE} --format=berkeley --totals "$<TARGET_FILE:${Project}>"
              COMMAND echo "")
    endif(GCC_SIZE)
  endmacro()

  ################################################################################
  # Convert output elf to hex
  ################################################################################
  macro( CcElfToImage Project)
    if(CCOS_ELF_TO_IMAGE_EXECUTABLE)
      add_custom_command(TARGET ${Project} POST_BUILD
              COMMENT "Invoking: Convert elf to hex"
              COMMAND echo ""
              COMMAND echo "Size of target hex file: "
              COMMAND echo ""
              COMMAND ${CMAKE_COMMAND} -E env "PATH=$ENV{PATH}"
                      python ${CCOS_ELF_TO_IMAGE_EXECUTABLE} elf2image "$<TARGET_FILE:${Project}>"
              COMMAND echo "")
    endif(CCOS_ELF_TO_IMAGE_EXECUTABLE)
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
                          "http://coolcow.de/projects/ThirdParty/WiXToolset/binaries/${WIX_VERSION}/wix-portable.7z")
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
    if(${Target} MATCHES "${StringToAdd}")
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
      message(WARNING "Git pull failed (${Git_EXTRACT_RESULT}): ${Repository}")
    endif()
  endmacro(CcGitUpdate Repository)

  macro(CcGitCheckout Repository Commit)
    execute_process(COMMAND git checkout ${Commit}
                    WORKING_DIRECTORY ${Repository}
                    RESULT_VARIABLE Git_EXTRACT_RESULT
                    #OUTPUT_QUIET ERROR_QUIET
                    )
    if(NOT ${Git_EXTRACT_RESULT} EQUAL 0)
      message(WARNING "Git checkout failed (${Git_EXTRACT_RESULT}): ${Commit} ${Repository}")
    endif()
  endmacro()

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
  # Add executable with makro to this project
  #
  ################################################################################
  macro(CcAddExecutable ProjectName Sources)
      set(AddExecutable_SOURCES ${Sources})
      foreach (_src ${ARGN})
        CcListAppendOnce(AddExecutable_SOURCES "${_src}")
      endforeach()
      if(CC_APPLICATION_LIB)
	      if(NOT "${ProjectName}" STREQUAL "")
          add_library(${ProjectName} STATIC ${AddExecutable_SOURCES})
	      endif()
        add_library(${CC_APPLICATION_LIB} STATIC "${CCOS_DIR}/Resources/dummy.c")
        target_link_libraries(${CC_APPLICATION_LIB} ${ProjectName})
      else()
        add_executable(${ProjectName} ${AddExecutable_SOURCES})
      endif()
  endmacro()

  ################################################################################
  # Add string resource to project
  ################################################################################
  macro(CcAddResourceString ResourceDir ResourceFileName VariableName FileList)
    add_custom_command(OUTPUT ${ResourceDir}/${ResourceFileName}.c
                              ${ResourceDir}/${ResourceFileName}.h
                              COMMAND CcOSResource -i ${ResourceFileName} -o ${ResourceFileName} -n ${VariableName} -s
                              DEPENDS ${ResourceDir}/${ResourceFileName}
                              WORKING_DIRECTORY ${ResourceDir}
    )
    CcListAppendOnce( ${FileList} ${ResourceDir}/${ResourceFileName}.c)
    CcListAppendOnce( ${FileList} ${ResourceDir}/${ResourceFileName}.h)
  endmacro()

  ################################################################################
  # Add binary resource to project
  ################################################################################
  macro(CcAddResourceBinary ResourceDir ResourceFileName VariableName FileList)
    add_custom_command(OUTPUT ${ResourceDir}/${ResourceFileName}.c
                              ${ResourceDir}/${ResourceFileName}.h
                              COMMAND CcOSResource -i ${ResourceFileName} -o ${ResourceFileName} -n ${VariableName}
                              DEPENDS ${ResourceDir}/${ResourceFileName}
                              WORKING_DIRECTORY ${ResourceDir}
    )
    CcListAppendOnce( ${FileList} ${ResourceDir}/${ResourceFileName}.c)
    CcListAppendOnce( ${FileList} ${ResourceDir}/${ResourceFileName}.h)
  endmacro()

endif(NOT CC_MACRO_LOADED)
