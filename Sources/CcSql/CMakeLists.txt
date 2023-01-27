set ( CURRENT_PROJECT           CcSql )
set ( CURRENT_PROJECT_IDE_PATH   )

########
# Add Header Files
########
set( HEADER_FILES
  CcSql.h
  ISqlDatabase.h
  CcSqlResult.h
  CcSqlDatabase.h
)

########
# Add Source Files
########
set( SOURCE_FILES
  ${HEADER_FILES}
  ISqlDatabase.cpp
  CcSqlResult.cpp
  CcSqlDatabase.cpp
)

set( LIBRARIES
  CcKernel
)

set( DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} )


if( WINDOWS AND NOT GCC)
list( APPEND SOURCE_FILES "CcWmiInterface.cpp")
list( APPEND HEADER_FILES "CcWmiInterface.h")
#for mingw for testing list( APPEND LIBRARIES    libcomsuppw )
endif()

if( CCOS_THIRDPARTY_SQLITE3_ACTIVE )
list( APPEND SOURCE_FILES "CcSqlite.cpp")
list( APPEND HEADER_FILES "CcSqlite.h")
list( APPEND LIBRARIES    CcOS::ThirParty::sqlite3 )
endif()

include_directories( ${DIRECTORIES} )

if(WINDOWS)
CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
CcTargetHeaders( ${CURRENT_PROJECT} "${HEADER_FILES}")

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )

#################
# Post configuration steps
#################
CcTargetIncludes( ${CURRENT_PROJECT} )
CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
