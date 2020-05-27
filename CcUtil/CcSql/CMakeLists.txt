if(NOT DEFINED CCOS_CCUTIL_CCSQL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCUTIL_CCSQL_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcSql )
  set ( CURRENT_PROJECT_IDE_PATH  CcUtil )

  ########
  # Add Source Files
  ########
  set ( SOURCE_FILES
        "ISqlDatabase.cpp"
        "CcSqlResult.cpp"
        "CcSqlDatabase.cpp"
        "CcSql.h"
        "ISqlDatabase.h"
        "CcSqlResult.h"
        "CcSqlDatabase.h")

  set( LIBRARIES
        CcKernel )

  set( DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} )


  if( WINDOWS AND NOT GCC)
      list( APPEND SOURCE_FILES "CcWmiInterface.cpp")
      list( APPEND HEADER_FILES "CcWmiInterface.h")
      #for mingw for testing list( APPEND LIBRARIES    libcomsuppw )
  endif()

  if( CCOS_THIRDPARTY_SQLITE3_ACTIVE )
      list( APPEND SOURCE_FILES "CcSqlite.cpp")
      list( APPEND HEADER_FILES "CcSqlite.h")
      list( APPEND LIBRARIES    ${SQLite3_LIBRARIES} )
      list( APPEND DIRECTORIES  ${SQLite3_INCLUDE_DIR} )
      add_definitions(-DCCOS_THIRDPARTY_SQLITE )
  endif()

  include_directories( ${DIRECTORIES} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
endif()
