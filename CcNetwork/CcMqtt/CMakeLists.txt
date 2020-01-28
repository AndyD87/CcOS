if(NOT DEFINED CCOS_CCNETWORK_CCMQTT_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCNETWORK_CCMQTT_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcMqtt )
  set ( CURRENT_PROJECT_IDE_PATH  CcNetwork )

  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.c"
        "*.cpp"
        "config/*.xml"
        "*.h"
  )

  set(CcMqtt_LIBRARIES CcKernel)
  set(CcMqtt_DEFINITIONS )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  if( CCOS_THIRDPARTY_OPENSSL_ACTIVE GREATER ${CCOS_BUILDLEVEL} )
    list( APPEND CcMqtt_DEFINITIONS -DCCSSL_ENABLED)
    list( APPEND CcMqtt_LIBRARIES CcSsl)
  endif()

  add_definitions(${CcMqtt_DEFINITIONS})
  add_library( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES})

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                                ${CcMqtt_LIBRARIES} )

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
