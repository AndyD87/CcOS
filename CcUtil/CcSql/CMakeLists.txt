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
  #    # search sqlite, if failed ThirdPartyConfig should have created the project
  #    if( NOT DEFINED WIN32)
  #      set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC")
  #      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  #    endif()
  #
  #    find_package( sqlite3 QUIET )
      list( APPEND SOURCE_FILES "src/CcSqlite.cpp")
      list( APPEND HEADER_FILES "CcSqlite.h")
      list( APPEND LIBRARIES    sqlite3 )
      add_definitions(-DCCOS_THIRDPARTY_SQLITE )
  endif()
  
  include_directories( ${DIRECTORIES} )
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT})
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
  endif()
  
  add_library( ${CURRENT_PROJECT} ${CCOS_LINK_TYPE} ${SOURCE_FILES} )
  
  # To keep Header an Source together, display them in same Directory
  # It will only work with IDE's wich are supporting this features
  SOURCE_GROUP( ""        FILES ${SOURCE_FILES})
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcUtil")
  
  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )
      
  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSSetInstall( ${CURRENT_PROJECT} )

endif()