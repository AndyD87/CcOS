if(NOT DEFINED CCOS_THIRDPARTY_SQLITE3_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_THIRDPARTY_SQLITE3_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  #find_package ( sqlite3 QUIET )
  
  #if(sqlite3_FOUND)
  #  message("- SQLite3 found")
  #else()
    set(CURRENT_PROJECT sqlite3)
    message("- SQLite3 not found, create project")
  
    include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  
    if( NOT DEFINED WIN32)
      set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    endif()
  
    add_library( ${CURRENT_PROJECT} STATIC sqlite3.c sqlite3.h )
  
    set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/ThirdParty")
  
    if( NOT DEFINED WIN32)
      target_link_libraries( ${CURRENT_PROJECT} LINK_PUBLIC "dl")
    endif()
  
    #################
    # Post configuration steps
    #################
    CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
    CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE)
  #endif()
endif()