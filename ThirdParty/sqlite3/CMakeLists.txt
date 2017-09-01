if(NOT DEFINED CCOS_THIRDPARTY_SQLITE3_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_THIRDPARTY_SQLITE3_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  #find_package ( sqlite3 QUIET )
  
  #if(sqlite3_FOUND)
  #  MESSAGE("  SQLite3 found")
  #else()
    MESSAGE("  SQLite3 not found, create project")
  
    include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  
    MESSAGE( "${CMAKE_CURRENT_SOURCE_DIR}" )
  
    if( NOT DEFINED WIN32)
      set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    endif()
  
    add_library( sqlite3 STATIC sqlite3.c sqlite3.h )
  
    set_property( TARGET sqlite3 PROPERTY FOLDER "CcOS/ThirdParty")
  
    if( NOT DEFINED WIN32)
      target_link_libraries( sqlite3 LINK_PUBLIC "dl")
    endif()
  
    #################
    # Post configuration steps
    #################
    CcOSTargetIncludeDirs( sqlite3 )
    CcOSSetInstall( sqlite3 )
  #endif()
endif()