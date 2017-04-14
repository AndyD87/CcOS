
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

if( USE_THIRDPARTY_SQLITE )
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
    add_definitions(-DUSE_THIRDPARTY_SQLITE )
endif()

MESSAGE("${LIBRARIES}")

include_directories( ${DIRECTORIES} )

add_library( ${CURRENT_PROJECT} ${LINK_TYPE} ${SOURCE_FILES} )

SOURCE_GROUP( ""        FILES ${SOURCE_FILES})

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcUtil")

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )
    
#################
# Post configuration steps
#################
CcTargetIncludeDirs( ${CURRENT_PROJECT} )
CcSetInstall( ${CURRENT_PROJECT} )
