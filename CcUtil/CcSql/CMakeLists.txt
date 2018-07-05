if(NOT DEFINED CCOS_CCUTIL_CCSQL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCUTIL_CCSQL_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcSql )
  
  ########
  # Add Source Files
  ########
  set ( SOURCE_FILES
        "src/CcISqlDatabase.cpp"
        "src/CcSqlResult.cpp"
        "src/CcSqlDatabase.cpp"
        "CcSql.h"
        "CcISqlDatabase.h"
        "CcSqlResult.h"
        "CcSqlDatabase.h")
  
  set( LIBRARIES
        CcKernel )
        
  set( DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} )
  
  
  if( WIN32 )
      list( APPEND SOURCE_FILES "src/CcWmiInterface.cpp")
      list( APPEND HEADER_FILES "CcWmiInterface.h")
  endif()
  
  if( CCOS_THIRDPARTY_SQLITE )
      list( APPEND SOURCE_FILES "src/CcSqlite.cpp")
      list( APPEND HEADER_FILES "CcSqlite.h")
      list( APPEND LIBRARIES    ${sqlite3_LIBRARIES} )
      list( APPEND DIRECTORIES  ${sqlite3_INCLUDE_DIR} )
      add_definitions(-DCCOS_THIRDPARTY_SQLITE )
  endif()
  
  include_directories( ${DIRECTORIES} )
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()
  
  add_library( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcUtil")
  
  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )
      
  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
endif()