if(NOT DEFINED CCOS_CCNETWORK_CCDHCP_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCNETWORK_CCDHCP_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcDhcp )
  set ( CURRENT_PROJECT_IDE_PATH  CcNetwork )

  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
          CcDhcp.h
          CcDhcpGlobals.cpp
          CcDhcpGlobals.h
          CcDhcpLeaseItem.cpp
          CcDhcpLeaseItem.h
          CcDhcpLeaseList.cpp
          CcDhcpLeaseList.h
          CcDhcpServer.cpp
          CcDhcpServer.h
          config/default.json
          Server/CConfig.cpp
          Server/CConfig.h
          Server/CData.cpp
          Server/CData.h
          Server/CWorker.cpp
          Server/CWorker.h
  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES})

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                                  CcKernel
                                  CcDocuments )


  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
endif()
