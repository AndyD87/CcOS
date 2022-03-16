if(NOT DEFINED CCOS_CCDHCP_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCDHCP_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcDhcp )
  set ( CURRENT_PROJECT_IDE_PATH   )

  ########
  # Add Header Files
  ########
  set( HEADER_FILES
          CcDhcp.h
          CcDhcpGlobals.h
          CcDhcpLeaseItem.h
          CcDhcpLeaseList.h
          CcDhcpServer.h
          Server/CConfig.h
          Server/CData.h
          Server/CWorker.h
  )

  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
          ${HEADER_FILES}
          CcDhcpGlobals.cpp
          CcDhcpLeaseItem.cpp
          CcDhcpLeaseList.cpp
          CcDhcpServer.cpp
          config/default.json
          Server/CConfig.cpp
          Server/CData.cpp
          Server/CWorker.cpp
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
                                  CcDocuments )


  #################
  # Post configuration steps
  #################
  CcTargetIncludes( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
endif()
