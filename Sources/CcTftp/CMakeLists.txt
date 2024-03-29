set ( CURRENT_PROJECT           CcTftp )
set ( CURRENT_PROJECT_IDE_PATH   )

########
# Add Header Files
########
set( HEADER_FILES
      CcTftpClient.h
      CcTftpGlobals.h
      CcTftp.h
      CcTftpServerConfig.h
      CcTftpServer.h
      CcTftpServerWorker.h
      ETftpServer.h
)

########
# Add Source Files
########
set( SOURCE_FILES
      ${HEADER_FILES}
      CcTftpClient.cpp
      CcTftpGlobals.cpp
      CcTftpServer.cpp
      CcTftpServerConfig.cpp
      CcTftpServerWorker.cpp
      config/default.xml
)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES})

set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
CcTargetHeaders( ${CURRENT_PROJECT} "${HEADER_FILES}")

target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                              CcKernel
)

#################
# Post configuration steps
#################
CcTargetIncludes( ${CURRENT_PROJECT} )
CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
