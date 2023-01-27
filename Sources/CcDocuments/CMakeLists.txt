set ( CURRENT_PROJECT           CcDocuments )
set ( CURRENT_PROJECT_IDE_PATH   )

########
# Add Header Files
########
set( HEADER_FILES
  CcDocument.h
  NDocumentsGlobals.h
  Config/CcConfig.h
  Config/CcConfigBinary.h
  Config/CcConfigDocument.h
  Config/CcConfigItem.h
  Config/CcConfigMap.h
  Html/CcHtml.h
  Html/CcHtmlAttribute.h
  Html/CcHtmlDocument.h
  Html/CcHtmlNode.h
  Html/CcHtmlNodeList.h
  Html/CcHtmlUtil.h
  Html/IHtmlImpl.h
  Html/Common/CcHtmlA.h
  Html/Common/CcHtmlB.h
  Html/Common/CcHtmlBr.h
  Html/Common/CcHtmlDiv.h
  Html/Common/CcHtmlForm.h
  Html/Common/CcHtmlH.h
  Html/Common/CcHtmlHead.h
  Html/Common/CcHtmlInput.h
  Html/Common/CcHtmlInputButton.h
  Html/Common/CcHtmlInputCheckbox.h
  Html/Common/CcHtmlInputFile.h
  Html/Common/CcHtmlInputHidden.h
  Html/Common/CcHtmlInputPassword.h
  Html/Common/CcHtmlInputRadio.h
  Html/Common/CcHtmlInputSubmit.h
  Html/Common/CcHtmlInputText.h
  Html/Common/CcHtmlLabel.h
  Html/Common/CcHtmlP.h
  Html/Common/CcHtmlRoot.h
  Html/Common/CcHtmlScript.h
  Html/Common/CcHtmlSelect.h
  Html/Common/CcHtmlTable.h
  Html/Common/CcHtmlTextArea.h
  Json/CcJson.h
  Json/CcJsonArray.h
  Json/CcJsonDocument.h
  Json/CcJsonFile.h
  Json/CcJsonNode.h
  Json/CcJsonObject.h
  Xml/CcXml.h
  Xml/CcXmlConfigFile.h
  Xml/CcXmlDocument.h
  Xml/CcXmlFile.h
  Xml/CcXmlNode.h
  Xml/CcXmlNodeList.h
  Xml/CcXmlUtil.h
)

########
# Add Source Files
########
set( SOURCE_FILES
  ${HEADER_FILES}
  NDocumentsGlobals.cpp
  Config/CcConfigBinary.cpp
  Config/CcConfigDocument.cpp
  Config/CcConfigItem.cpp
  Config/CcConfigMap.cpp
  Html/CcHtmlAttribute.cpp
  Html/CcHtmlDocument.cpp
  Html/CcHtmlNode.cpp
  Html/CcHtmlUtil.cpp
  Html/IHtmlImpl.cpp
  Html/Common/CcHtmlA.cpp
  Html/Common/CcHtmlB.cpp
  Html/Common/CcHtmlBr.cpp
  Html/Common/CcHtmlDiv.cpp
  Html/Common/CcHtmlForm.cpp
  Html/Common/CcHtmlH.cpp
  Html/Common/CcHtmlHead.cpp
  Html/Common/CcHtmlInput.cpp
  Html/Common/CcHtmlInputButton.cpp
  Html/Common/CcHtmlInputCheckbox.cpp
  Html/Common/CcHtmlInputFile.cpp
  Html/Common/CcHtmlInputHidden.cpp
  Html/Common/CcHtmlInputPassword.cpp
  Html/Common/CcHtmlInputRadio.cpp
  Html/Common/CcHtmlInputSubmit.cpp
  Html/Common/CcHtmlInputText.cpp
  Html/Common/CcHtmlLabel.cpp
  Html/Common/CcHtmlP.cpp
  Html/Common/CcHtmlRoot.cpp
  Html/Common/CcHtmlScript.cpp
  Html/Common/CcHtmlSelect.cpp
  Html/Common/CcHtmlTable.cpp
  Html/Common/CcHtmlTextArea.cpp
  Json/CcJsonArray.cpp
  Json/CcJsonDocument.cpp
  Json/CcJsonFile.cpp
  Json/CcJsonNode.cpp
  Json/CcJsonObject.cpp
  Xml/CcXmlConfigFile.cpp
  Xml/CcXmlDocument.cpp
  Xml/CcXmlFile.cpp
  Xml/CcXmlNode.cpp
  Xml/CcXmlUtil.cpp
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
