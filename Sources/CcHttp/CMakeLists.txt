if(NOT DEFINED CCOS_CCHTTP_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCHTTP_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT           "CcHttp" )
  set ( CURRENT_PROJECT_IDE_PATH  ""       )

  ########
  # Add Header Files
  ########
  set( HEADER_FILES
          CcHttpClient.h
          CcHttpCookies.h
          CcHttpDefaults.h
          CcHttpGlobals.h
          CcHttpGlobalStrings.h
          CcHttp.h
          CcHttpRequest.h
          CcHttpResponse.h
          CcHttpServerConfig.h
          CcHttpServer.h
          CcHttpServerWorker.h
          CcHttpTransferEncoding.h
          CcHttpWorkData.h
          IHttpProvider.h
          IHttpUserControl.h
          IHttpUser.h

          Applications/RestApiWebframework/CcHttpWebframeworkCssProvider.h
          Applications/RestApiWebframework/CcHttpWebframework.h
          Applications/RestApiWebframework/CcHttpWebframeworkIndex.h
          Applications/RestApiWebframework/CcHttpWebframeworkJsProvider.h
          Applications/RestApiWebframework/CcHttpWebframeworkPage.h

          RestApi/CcRestApiApplicationFooter.h
          RestApi/CcRestApiApplication.h
          RestApi/CcRestApiApplicationMenu.h
          RestApi/CcRestApiApplicationStatus.h
          RestApi/CcRestApiDeviceGpioPin.h
          RestApi/CcRestApiDevice.h
          RestApi/CcRestApiDevices.h
          RestApi/CcRestApiDeviceWlan.h
          RestApi/CcRestApiMemory.h
          RestApi/CcRestApiMemoryMonitor.h
          RestApi/CcRestApiSystem.h
          RestApi/CcRestApiUser.h
          RestApi/IRestApi.h

          Resources/CcOSWebframework.css.h
          Resources/CcOSWebframework.js.h

          HttpProvider/CcHttpCamera.h
          HttpProvider/CcHttpDefaultProvider.h
          HttpProvider/CcHttpRestApiProvider.h
          HttpProvider/IHttpPathContentProvider.h
          HttpProvider/IHttpPathProvider.h
  )

  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
          ${HEADER_FILES}
          CcHttpClient.cpp
          CcHttpCookies.cpp
          CcHttpDefaults.cpp
          CcHttpGlobals.cpp
          CcHttpGlobalStrings.cpp
          CcHttpRequest.cpp
          CcHttpResponse.cpp
          CcHttpServerConfig.cpp
          CcHttpServer.cpp
          CcHttpServerWorker.cpp
          CcHttpTransferEncoding.cpp
          CcHttpWorkData.cpp
          IHttpProvider.cpp
          IHttpUserControl.cpp
          IHttpUser.cpp

          Applications/RestApiWebframework/CcHttpWebframework.cpp
          Applications/RestApiWebframework/CcHttpWebframeworkCssProvider.cpp
          Applications/RestApiWebframework/CcHttpWebframeworkIndex.cpp
          Applications/RestApiWebframework/CcHttpWebframeworkJsProvider.cpp
          Applications/RestApiWebframework/CcHttpWebframeworkPage.cpp

          RestApi/CcRestApiApplication.cpp
          RestApi/CcRestApiApplicationFooter.cpp
          RestApi/CcRestApiApplicationMenu.cpp
          RestApi/CcRestApiApplicationStatus.cpp
          RestApi/CcRestApiDevice.cpp
          RestApi/CcRestApiDeviceGpioPin.cpp
          RestApi/CcRestApiDevices.cpp
          RestApi/CcRestApiDeviceWlan.cpp
          RestApi/CcRestApiMemory.cpp
          RestApi/CcRestApiMemoryMonitor.cpp
          RestApi/CcRestApiSystem.cpp
          RestApi/CcRestApiUser.cpp
          RestApi/IRestApi.cpp

          Resources/CcOSWebframework.css
          Resources/CcOSWebframework.css.c
          Resources/CcOSWebframework.js.c

          HttpProvider/CcHttpCamera.cpp
          HttpProvider/CcHttpDefaultProvider.cpp
          HttpProvider/CcHttpRestApiProvider.cpp
          HttpProvider/IHttpPathContentProvider.cpp
          HttpProvider/IHttpPathProvider.cpp
  )

  set( DEFINITIONS "" )
  set( LIBRARIES  CcKernel CcDocuments )

  if( CCOS_THIRDPARTY_OPENSSL_ACTIVE GREATER ${CCOS_BUILDLEVEL} )
    list( APPEND LIBRARIES CcSsl)
    list( APPEND DEFINITIONS -DCCSSL_ENABLED)
  endif()

  add_definitions( ${DEFINITIONS} )

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

  if(CcOSResource_FOUND AND NOT ${CcOSResource_FOUND} STREQUAL "CcOSResource-NOTFOUND")
      CcAddResourceString(${CMAKE_CURRENT_LIST_DIR}/Resources CcOSWebframework.js CcOSWebframework_Js CURRENT_PROJECT_RESOURCE_FILES)
      CcAddResourceString(${CMAKE_CURRENT_LIST_DIR}/Resources CcOSWebframework.css CcOSWebframework_Css CURRENT_PROJECT_RESOURCE_FILES)

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

  set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  CcTargetHeaders( ${CURRENT_PROJECT} "${HEADER_FILES}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )


  #################
  # Post configuration steps
  #################
  CcTargetIncludes( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
  CcSetFiltersByFolders(${SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
