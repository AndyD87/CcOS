set ( CURRENT_PROJECT           CcMtp )
set ( CURRENT_PROJECT_IDE_PATH   )

if(MTP_FOUND)
  ########
  # Add Header Files
  ########
  set( HEADER_FILES
        CcMtpDevice.h
        CcMtp.h
  )

  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${HEADER_FILES}
        CcMtpDevice.cpp
  )

  add_definitions(${MTP_DEFINITIONS})

  include_directories( ${CMAKE_CURRENT_LIST_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

  set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  CcTargetHeaders( ${CURRENT_PROJECT} "${HEADER_FILES}")

  target_link_libraries ( ${CURRENT_PROJECT} PUBLIC
                                CcKernel
                                CcMedia
                                ${MTP_LIBRARIES} )

  #################
  # Post configuration steps
  #################
  CcTargetIncludes( ${CURRENT_PROJECT} ${MTP_INCLUDE_DIR})
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
endif()
