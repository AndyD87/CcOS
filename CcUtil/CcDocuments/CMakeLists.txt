if(NOT DEFINED CCOS_CCUTIL_CCDOCUMENTS_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCUTIL_CCDOCUMENTS_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcDocuments )
  set ( CURRENT_PROJECT_IDE_PATH  CcUtil )

  ########
  # Add Source Files
  ########
  set (SOURCE_FILES
        CcConfig
        CcDocument.h
        NDocumentsGlobals.cpp
        NDocumentsGlobals.h
        CcConfig/CcConfig.h
        CcConfig/CcConfigBinary.cpp
        CcConfig/CcConfigBinary.h
        CcConfig/CcConfigDocument.cpp
        CcConfig/CcConfigDocument.h
        CcConfig/CcConfigItem.cpp
        CcConfig/CcConfigItem.h
        CcConfig/CcConfigMap.cpp
        CcConfig/CcConfigMap.h
        CcHtml/CcHtml.h
        CcHtml/CcHtmlAttribute.cpp
        CcHtml/CcHtmlAttribute.h
        CcHtml/CcHtmlDocument.cpp
        CcHtml/CcHtmlDocument.h
        CcHtml/CcHtmlNode.cpp
        CcHtml/CcHtmlNode.h
        CcHtml/CcHtmlNodeList.h
        CcHtml/CcHtmlUtil.cpp
        CcHtml/CcHtmlUtil.h
        CcHtml/Common
        CcHtml/IHtmlImpl.cpp
        CcHtml/IHtmlImpl.h
        CcHtml/Common/CcHtmlA.cpp
        CcHtml/Common/CcHtmlA.h
        CcHtml/Common/CcHtmlB.cpp
        CcHtml/Common/CcHtmlB.h
        CcHtml/Common/CcHtmlBr.cpp
        CcHtml/Common/CcHtmlBr.h
        CcHtml/Common/CcHtmlDiv.cpp
        CcHtml/Common/CcHtmlDiv.h
        CcHtml/Common/CcHtmlForm.cpp
        CcHtml/Common/CcHtmlForm.h
        CcHtml/Common/CcHtmlH.cpp
        CcHtml/Common/CcHtmlH.h
        CcHtml/Common/CcHtmlHead.cpp
        CcHtml/Common/CcHtmlHead.h
        CcHtml/Common/CcHtmlInput.cpp
        CcHtml/Common/CcHtmlInput.h
        CcHtml/Common/CcHtmlInputButton.cpp
        CcHtml/Common/CcHtmlInputButton.h
        CcHtml/Common/CcHtmlInputCheckbox.cpp
        CcHtml/Common/CcHtmlInputCheckbox.h
        CcHtml/Common/CcHtmlInputFile.cpp
        CcHtml/Common/CcHtmlInputFile.h
        CcHtml/Common/CcHtmlInputHidden.cpp
        CcHtml/Common/CcHtmlInputHidden.h
        CcHtml/Common/CcHtmlInputPassword.cpp
        CcHtml/Common/CcHtmlInputPassword.h
        CcHtml/Common/CcHtmlInputRadio.cpp
        CcHtml/Common/CcHtmlInputRadio.h
        CcHtml/Common/CcHtmlInputSubmit.cpp
        CcHtml/Common/CcHtmlInputSubmit.h
        CcHtml/Common/CcHtmlInputText.cpp
        CcHtml/Common/CcHtmlInputText.h
        CcHtml/Common/CcHtmlLabel.cpp
        CcHtml/Common/CcHtmlLabel.h
        CcHtml/Common/CcHtmlP.cpp
        CcHtml/Common/CcHtmlP.h
        CcHtml/Common/CcHtmlRoot.cpp
        CcHtml/Common/CcHtmlRoot.h
        CcHtml/Common/CcHtmlScript.cpp
        CcHtml/Common/CcHtmlScript.h
        CcHtml/Common/CcHtmlSelect.cpp
        CcHtml/Common/CcHtmlSelect.h
        CcHtml/Common/CcHtmlTable.cpp
        CcHtml/Common/CcHtmlTable.h
        CcHtml/Common/CcHtmlTextArea.cpp
        CcHtml/Common/CcHtmlTextArea.h
        CcJson/CcJson.h
        CcJson/CcJsonArray.cpp
        CcJson/CcJsonArray.h
        CcJson/CcJsonDocument.cpp
        CcJson/CcJsonDocument.h
        CcJson/CcJsonFile.cpp
        CcJson/CcJsonFile.h
        CcJson/CcJsonNode.cpp
        CcJson/CcJsonNode.h
        CcJson/CcJsonObject.cpp
        CcJson/CcJsonObject.h
        CcXml/CcXml.h
        CcXml/CcXmlConfigFile.cpp
        CcXml/CcXmlConfigFile.h
        CcXml/CcXmlDocument.cpp
        CcXml/CcXmlDocument.h
        CcXml/CcXmlFile.cpp
        CcXml/CcXmlFile.h
        CcXml/CcXmlNode.cpp
        CcXml/CcXmlNode.h
        CcXml/CcXmlNodeList.h
        CcXml/CcXmlUtil.cpp
        CcXml/CcXmlUtil.h
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

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE ${LIBRARIES} )


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
