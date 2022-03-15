if(NOT DEFINED CCOS_CCFTP_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCFTP_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcFtp )
  set ( CURRENT_PROJECT_IDE_PATH   )

  ########
  # Add Header Files
  ########
  set( HEADER_FILES
          CcFtpClient.h
          CcFtp.h
          CcFtpServer.h
          CcFtpServerWorker.h
          CcFtpTypes.h
  )

  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
          ${HEADER_FILES}
          CcFtpClient.cpp
          CcFtpServer.cpp
          CcFtpServerWorker.cpp
          CcFtpTypes.cpp
  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

  set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  set_target_properties( ${CURRENT_PROJECT} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                                CcKernel )

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
endif()
