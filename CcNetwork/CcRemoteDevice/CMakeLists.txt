if(NOT DEFINED CCOS_CCNETWORK_CCREMOTEDEVICE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCNETWORK_CCREMOTEDEVICE_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           "CcRemoteDevice" )
  set ( CURRENT_PROJECT_IDE_PATH  "CcNetwork" )

  #############################################################################
  # Add source Files
  #############################################################################
  set(SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDevice.h
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceBoardSupport.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceBoardSupport.h
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceCssProvider.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceCssProvider.h
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceDiscovery.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceDiscovery.h
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceGlobals.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceGlobals.h
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceJsProvider.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceJsProvider.h
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceServer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceServer.h
        ${CMAKE_CURRENT_LIST_DIR}/Packets/CPaket.h
        ${CMAKE_CURRENT_LIST_DIR}/Packets/CRequestDiscover.h
        ${CMAKE_CURRENT_LIST_DIR}/Packets/CRequestInfo.h
        ${CMAKE_CURRENT_LIST_DIR}/Packets/CResponseDiscover.h
        ${CMAKE_CURRENT_LIST_DIR}/Packets/CResponseInfo.h
        ${CMAKE_CURRENT_LIST_DIR}/Server/CConfig.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Server/CConfig.h
        ${CMAKE_CURRENT_LIST_DIR}/Server/CWorker.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Server/CWorker.h
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CEvents.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CEvents.h
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CInterfaces.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CInterfaces.h
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CRestApi.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CRestApi.h
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CStartup.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CStartup.h
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CSystem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CSystem.h
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CUsers.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CUsers.h
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CWlanAccessPoint.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CWlanAccessPoint.h
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CWlanClient.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Server/Config/CWlanClient.h
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

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                  CcHttp
                                  CcDocuments)

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} ${OPENSLL_INCLUDE_DIR} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
  CcSetFiltersByFolders(${SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
