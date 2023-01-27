set ( CURRENT_PROJECT           CcMqtt )
set ( CURRENT_PROJECT_IDE_PATH   )

########
# Add Header Files
########
set( HEADER_FILES
      CcMqttClientConfig.h
      CcMqttClient.h
      CcMqttGlobals.h
      CcMqtt.h
)

########
# Add Source Files
########
set( SOURCE_FILES
      ${HEADER_FILES}
      CcMqttClientConfig.cpp
      CcMqttClient.cpp
      CcMqttGlobals.cpp
)

set(CcMqtt_LIBRARIES CcKernel)
set(CcMqtt_DEFINITIONS )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

if(TARGET CcSsl)
  list( APPEND CcMqtt_DEFINITIONS -DCCSSL_ENABLED)
  list( APPEND CcMqtt_LIBRARIES CcSsl)
endif()

add_definitions(${CcMqtt_DEFINITIONS})
CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES})

set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
CcTargetHeaders( ${CURRENT_PROJECT} "${HEADER_FILES}")

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              ${CcMqtt_LIBRARIES} )

#################
# Post configuration steps
#################
CcTargetIncludes( ${CURRENT_PROJECT} )
CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})

########
# Add an unity-test, if available
########
CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
