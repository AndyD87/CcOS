if(NOT DEFINED CCOS_THIRDPARTY_SQLITE3_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_THIRDPARTY_SQLITE3_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  find_package ( sqlite3 QUIET )
  
  if(sqlite3_FOUND)
    message("- SQLite3 found")
    set(sqlite3_LIBRARIES    "sqlite3"              PARENT_SCOPE)
    set(sqlite3_INCLUDE_DIR  ${sqlite3_INCLUDE_DIR} PARENT_SCOPE)
    set(sqlite3_LIB_DIR      ${sqlite3_LIB_DIR}     PARENT_SCOPE)
  else()
    set(sqlite3_VERSION_MAJOR     3   )
    set(sqlite3_VERSION_MINOR     21  )
    set(sqlite3_VERSION_PATCH     0   )
    set(sqlite3_VERSION_BUILD     0    )
    
    set(sqlite3_VERSION_CMAKE     ${sqlite3_VERSION_MAJOR}.${sqlite3_VERSION_MINOR}.${sqlite3_VERSION_PATCH})
    
    set(SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.c")
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.h")
    
    set(CURRENT_PROJECT sqlite3)
    
    message("- SQLite3 not found, create project")
  
    include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  
    if( DEFINED WIN32)
      configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/Sqlite3Version.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/Sqlite3Version.rc.tmp @ONLY)
      CcCopyFile(${CMAKE_CURRENT_SOURCE_DIR}/Sqlite3Version.rc.tmp ${CMAKE_CURRENT_SOURCE_DIR}/Sqlite3Version.rc)
      list(APPEND SOURCE_FILES   "${CMAKE_CURRENT_SOURCE_DIR}/Sqlite3Version.rc")
      if(${CC_LINK_TYPE} STREQUAL "SHARED")
        set(TEMP_DEFINE "__declspec(dllexport)")
        add_definitions(-DSQLITE_API=${TEMP_DEFINE})
      endif()
    elseif()
      set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    endif()
  
    add_library( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )
    
    set_target_properties(  ${ProjectName} PROPERTIES
                            VERSION sqlite3_VERSION_CMAKE
                            SOVERSION sqlite3_VERSION_CMAKE)
  
    set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/ThirdParty")
  
    if( NOT DEFINED WIN32)
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
    set(sqlite3_LIBRARIES    sqlite3                      PARENT_SCOPE)
    set(sqlite3_INCLUDE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}  PARENT_SCOPE)
    set(sqlite3_LIB_DIR      ${sqlite3_LIB_DIR}           PARENT_SCOPE)
  endif()
endif()