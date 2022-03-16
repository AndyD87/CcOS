if(NOT DEFINED CCOS_CCDOCUMENTS_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCDOCUMENTS_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcDocuments )
  set ( CURRENT_PROJECT_IDE_PATH   )

  ########
  # Add Header Files
  ########
  set( HEADER_FILES
        CcDocument.h
        NDocumentsGlobals.h
        CcConfig/CcConfig.h
        CcConfig/CcConfigBinary.h
        CcConfig/CcConfigDocument.h
        CcConfig/CcConfigItem.h
        CcConfig/CcConfigMap.h
        CcHtml/CcHtml.h
        CcHtml/CcHtmlAttribute.h
        CcHtml/CcHtmlDocument.h
        CcHtml/CcHtmlNode.h
        CcHtml/CcHtmlNodeList.h
        CcHtml/CcHtmlUtil.h
        CcHtml/IHtmlImpl.h
        CcHtml/Common/CcHtmlA.h
        CcHtml/Common/CcHtmlB.h
        CcHtml/Common/CcHtmlBr.h
        CcHtml/Common/CcHtmlDiv.h
        CcHtml/Common/CcHtmlForm.h
        CcHtml/Common/CcHtmlH.h
        CcHtml/Common/CcHtmlHead.h
        CcHtml/Common/CcHtmlInput.h
        CcHtml/Common/CcHtmlInputButton.h
        CcHtml/Common/CcHtmlInputCheckbox.h
        CcHtml/Common/CcHtmlInputFile.h
        CcHtml/Common/CcHtmlInputHidden.h
        CcHtml/Common/CcHtmlInputPassword.h
        CcHtml/Common/CcHtmlInputRadio.h
        CcHtml/Common/CcHtmlInputSubmit.h
        CcHtml/Common/CcHtmlInputText.h
        CcHtml/Common/CcHtmlLabel.h
        CcHtml/Common/CcHtmlP.h
        CcHtml/Common/CcHtmlRoot.h
        CcHtml/Common/CcHtmlScript.h
        CcHtml/Common/CcHtmlSelect.h
        CcHtml/Common/CcHtmlTable.h
        CcHtml/Common/CcHtmlTextArea.h
        CcJson/CcJson.h
        CcJson/CcJsonArray.h
        CcJson/CcJsonDocument.h
        CcJson/CcJsonFile.h
        CcJson/CcJsonNode.h
        CcJson/CcJsonObject.h
        CcXml/CcXml.h
        CcXml/CcXmlConfigFile.h
        CcXml/CcXmlDocument.h
        CcXml/CcXmlFile.h
        CcXml/CcXmlNode.h
        CcXml/CcXmlNodeList.h
        CcXml/CcXmlUtil.h
  )

  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${HEADER_FILES}
        NDocumentsGlobals.cpp
        CcConfig/CcConfigBinary.cpp
        CcConfig/CcConfigDocument.cpp
        CcConfig/CcConfigItem.cpp
        CcConfig/CcConfigMap.cpp
        CcHtml/CcHtmlAttribute.cpp
        CcHtml/CcHtmlDocument.cpp
        CcHtml/CcHtmlNode.cpp
        CcHtml/CcHtmlUtil.cpp
        CcHtml/IHtmlImpl.cpp
        CcHtml/Common/CcHtmlA.cpp
        CcHtml/Common/CcHtmlB.cpp
        CcHtml/Common/CcHtmlBr.cpp
        CcHtml/Common/CcHtmlDiv.cpp
        CcHtml/Common/CcHtmlForm.cpp
        CcHtml/Common/CcHtmlH.cpp
        CcHtml/Common/CcHtmlHead.cpp
        CcHtml/Common/CcHtmlInput.cpp
        CcHtml/Common/CcHtmlInputButton.cpp
        CcHtml/Common/CcHtmlInputCheckbox.cpp
        CcHtml/Common/CcHtmlInputFile.cpp
        CcHtml/Common/CcHtmlInputHidden.cpp
        CcHtml/Common/CcHtmlInputPassword.cpp
        CcHtml/Common/CcHtmlInputRadio.cpp
        CcHtml/Common/CcHtmlInputSubmit.cpp
        CcHtml/Common/CcHtmlInputText.cpp
        CcHtml/Common/CcHtmlLabel.cpp
        CcHtml/Common/CcHtmlP.cpp
        CcHtml/Common/CcHtmlRoot.cpp
        CcHtml/Common/CcHtmlScript.cpp
        CcHtml/Common/CcHtmlSelect.cpp
        CcHtml/Common/CcHtmlTable.cpp
        CcHtml/Common/CcHtmlTextArea.cpp
        CcJson/CcJsonArray.cpp
        CcJson/CcJsonDocument.cpp
        CcJson/CcJsonFile.cpp
        CcJson/CcJsonNode.cpp
        CcJson/CcJsonObject.cpp
        CcXml/CcXmlConfigFile.cpp
        CcXml/CcXmlDocument.cpp
        CcXml/CcXmlFile.cpp
        CcXml/CcXmlNode.cpp
        CcXml/CcXmlUtil.cpp
  )

  set( LIBRARIES
        CcKernel
  )

  set (DOCUMENTS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

  include_directories( ${DOCUMENTS_INCLUDE_DIRS} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddLibrary(${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

  set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  CcTargetHeaders( ${CURRENT_PROJECT} "${HEADER_FILES}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE ${LIBRARIES} )


  #################
  # Post configuration steps
  #################
  CcTargetIncludes( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
