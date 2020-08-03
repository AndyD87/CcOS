if(NOT DEFINED CCOS_THIRDPARTY_SQLITE3_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_THIRDPARTY_SQLITE3_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set(CURRENT_PROJECT sqlite3)

  find_package ( SQLite3 QUIET )
  
  if(SQLite3_FOUND)
    message("- SQLite3 found")
    set(SQLite3_LIBRARIES    ${CURRENT_PROJECT}     PARENT_SCOPE)
    set(SQLite3_INCLUDE_DIR  ${SQLite3_INCLUDE_DIR} PARENT_SCOPE)
    set(SQLite3_LIB_DIR      ${SQLite3_LIB_DIR}     PARENT_SCOPE)
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
    set(SQLite3_VERSION_MINOR     26   )
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
    CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
    CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE)
    
    #################
    # Data for CcSql
    #################
    set(SQLite3_LIBRARIES    ${CURRENT_PROJECT}           PARENT_SCOPE)
    set(SQLite3_INCLUDE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}  PARENT_SCOPE)
    set(SQLite3_LIB_DIR      ${SQLite3_LIB_DIR}           PARENT_SCOPE)
  endif()
endif()
