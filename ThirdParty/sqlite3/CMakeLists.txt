if(NOT DEFINED CCOS_THIRDPARTY_SQLITE3_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_THIRDPARTY_SQLITE3_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set(CURRENT_PROJECT CcThirdPartySqlite3)

  find_package ( SQLite3 QUIET )

  if(SQLite3_FOUND)
    message(FATAL_ERROR "- SQLite3 found")
    add_library(${CURRENT_PROJECT} INTERFACE)
    target_include_directories(${CURRENT_PROJECT} INTERFACE ${SQLite3_INCLUDE_DIR})
    target_link_libraries     (${CURRENT_PROJECT} INTERFACE ${SQLite3_LIBRARIES})
    add_library(CcOS::ThirParty::sqlite3 ALIAS sqlit)
    CcTargetInstall(${CURRENT_PROJECT})
  else()
    if( DEFINED GCC )
      # avoid fall through warning for GCC
      if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER "7.0.0" OR
         "${CMAKE_CXX_COMPILER_VERSION}" VERSION_EQUAL "7.0.0")
        CcAppendCompilerFlags("-Wimplicit-fallthrough=0")
      endif()
    endif( DEFINED GCC )
    set(SQLite3_FOUND             TRUE )
    set(SQLite3_VERSION_MAJOR     3    )
    set(SQLite3_VERSION_MINOR    34    )
    set(SQLite3_VERSION_PATCH     0    )
    set(SQLite3_VERSION_BUILD     0    )

    set(SQLite3_VERSION_CMAKE     ${SQLite3_VERSION_MAJOR}.${SQLite3_VERSION_MINOR}.${SQLite3_VERSION_PATCH})

    set(SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.c")
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.h")

    message("- SQLite3 not found, create project")

    include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

    if( DEFINED WINDOWS)
      configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/Sqlite3Version.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/Sqlite3Version.rc.tmp @ONLY)
      CcMoveFile(${CMAKE_CURRENT_SOURCE_DIR}/Sqlite3Version.rc.tmp ${CMAKE_CURRENT_SOURCE_DIR}/Sqlite3Version.rc)
      if(NOT DEFINED GCC)
        list(APPEND SOURCE_FILES   "${CMAKE_CURRENT_SOURCE_DIR}/Sqlite3Version.rc")
      endif()
      if(${CC_LINK_TYPE} STREQUAL "SHARED")
        set(TEMP_DEFINE "__declspec(dllexport)")
        add_definitions(-DSQLITE_API=${TEMP_DEFINE})
      endif()
    elseif()
      set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    endif()

    CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

    set_target_properties(  ${ProjectName} PROPERTIES
                            VERSION SQLite3_VERSION_CMAKE
                            SOVERSION SQLite3_VERSION_CMAKE)

    set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/ThirdParty")

    if( NOT DEFINED WINDOWS)
      target_link_libraries( ${CURRENT_PROJECT} LINK_PUBLIC "dl")
    endif()

    #################
    # Post configuration steps
    #################
    CcTargetIncludes( ${CURRENT_PROJECT} )
    CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE)

    #################
    # Data for CcSql
    #################
    add_library(CcOS::ThirParty::sqlite3 ALIAS ${CURRENT_PROJECT})
  endif()
endif()
