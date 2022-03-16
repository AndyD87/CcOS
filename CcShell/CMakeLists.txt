if(NOT DEFINED CCOS_CCSHELL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCSHELL_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcShell )
  set ( CURRENT_PROJECT_IDE_PATH   )

  ########
  # Add Header Files
  ########
  set( HEADER_FILES
        CcShell.h
        CcTelnetServer.h
        CcTelnetServerWorker.h
  )

  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${HEADER_FILES}
        CcShell.cpp
        CcTelnetServer.cpp
        CcTelnetServerWorker.cpp
        config/default.xml
  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

  set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  CcTargetHeaders( ${CURRENT_PROJECT} "${HEADER_FILES}")

  target_link_libraries ( ${CURRENT_PROJECT} PUBLIC
                                CcKernel
  )

  #################
  # Post configuration steps
  #################
  CcTargetIncludes( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
endif()
