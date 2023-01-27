set ( CURRENT_PROJECT           "CcRemoteDevice" )
set ( CURRENT_PROJECT_IDE_PATH   )

########
# Add Header Files
########
set( HEADER_FILES
      CcRemoteDevice.h
      CcRemoteDeviceBoardSupport.h
      CcRemoteDeviceConnector.h
      CcRemoteDeviceCssProvider.h
      CcRemoteDeviceDiscovery.h
      CcRemoteDeviceGlobals.h
      CcRemoteDeviceJsProvider.h
      CcRemoteDeviceServer.h
      Packets/CPaket.h
      Packets/CRequestDiscover.h
      Packets/CRequestInfo.h
      Packets/CResponseDiscover.h
      Packets/CResponseInfo.h
      Server/CConfig.h
      Server/CDiscoveryServer.h
      Server/CDiscoveryServerWorker.h
      Server/CInterfaceServer.cpp
      Server/CInterfaceServer.h
      Server/CInterfaceServerThread.h
      Server/Config/CEvents.cpp
      Server/Config/CEvents.h
      Server/Config/CInterfaces.h
      Server/Config/CRestApi.h
      Server/Config/CStartup.h
      Server/Config/CSystem.h
      Server/Config/CUsers.h
      Server/Config/CWlanAccessPoint.h
      Server/Config/CWlanClient.h
)

########
# Add Source Files
########
set( SOURCE_FILES
      ${HEADER_FILES}
      CcRemoteDeviceBoardSupport.cpp
      CcRemoteDeviceConnector.cpp
      CcRemoteDeviceCssProvider.cpp
      CcRemoteDeviceDiscovery.cpp
      CcRemoteDeviceGlobals.cpp
      CcRemoteDeviceJsProvider.cpp
      CcRemoteDeviceServer.cpp
      Server/CConfig.cpp
      Server/CDiscoveryServer.cpp
      Server/CDiscoveryServerWorker.cpp
      Server/CInterfaceServerThread.cpp
      Server/Config/CInterfaces.cpp
      Server/Config/CRestApi.cpp
      Server/Config/CStartup.cpp
      Server/Config/CSystem.cpp
      Server/Config/CUsers.cpp
      Server/Config/CWlanAccessPoint.cpp
      Server/Config/CWlanClient.cpp
)

if(CCREMOTEDEVICE_CUSTOM_BOARDSUPPORT)

elseif(CCOS_DRIVER_CPU_ESPRESSIF_ESP8266)
  list(APPEND SOURCE_FILES "BoardSupport/ESP8266.cpp")
else()
  list(APPEND SOURCE_FILES "BoardSupport/None.cpp")
endif()

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

#############################################################################
# Add resources to this project
#############################################################################
find_program(CcOSResource_FOUND CcOSResource)
file (GLOB CURRENT_PROJECT_RESOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/Resources/*"
)

if(CcOSResource_FOUND)
    CcAddResourceString(${CMAKE_CURRENT_LIST_DIR}/Resources CcRemoteDevice.js CcRemoteDevice_Js CURRENT_PROJECT_RESOURCE_FILES)
    CcAddResourceString(${CMAKE_CURRENT_LIST_DIR}/Resources CcRemoteDevice.css CcRemoteDevice_Css CURRENT_PROJECT_RESOURCE_FILES)
    CcAddResourceString(${CMAKE_CURRENT_LIST_DIR}/Resources CcRemoteDeviceGeneric.json CcRemoteDeviceGeneric_json CURRENT_PROJECT_RESOURCE_FILES)

    add_custom_target("${CURRENT_PROJECT}_Resources" DEPENDS  ${CURRENT_PROJECT_RESOURCE_FILES})
    set_target_properties("${CURRENT_PROJECT}_Resources" PROPERTIES FOLDER "CcOS/Resources/${CURRENT_PROJECT_IDE_PATH}")
endif()

source_group("Resources" FILES ${CURRENT_PROJECT_RESOURCE_FILES})

#############################################################################
# Setup project
#############################################################################
CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} ${CURRENT_PROJECT_RESOURCE_FILES})

#############################################################################
# Setup project dependencies
#############################################################################
if(CcOSResource_FOUND)
  add_dependencies(${CURRENT_PROJECT} "${CURRENT_PROJECT}_Resources")
endif()

set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
CcTargetHeaders( ${CURRENT_PROJECT} "${HEADER_FILES}")

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                CcHttp
                                CcDocuments
)

#################
# Post configuration steps
#################
CcTargetIncludes( ${CURRENT_PROJECT})
CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
CcSetFiltersByFolders(${SOURCE_FILES})

########
# Add an unity-test, if available
########
CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
