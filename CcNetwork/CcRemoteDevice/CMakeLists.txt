if(NOT DEFINED CCOS_CCNETWORK_CCREMOTEDEVICE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCNETWORK_CCREMOTEDEVICE_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           "CcRemoteDevice" )
  set ( CURRENT_PROJECT_IDE_PATH  "CcNetwork" )

  #############################################################################
  # Add source Files
  #############################################################################
  file (GLOB SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/Server/Config/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/Server/Config/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Server/Config/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/Packets/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/Packets/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Packets/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/Server/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/Server/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Server/*.h"
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
      CcSetFiltersByFolders(${CURRENT_PROJECT_RESOURCE_FILES})
  endif()

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
