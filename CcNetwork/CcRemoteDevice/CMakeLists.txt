if(NOT DEFINED CCOS_CCNETWORK_CCREMOTEDEVICE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCNETWORK_CCREMOTEDEVICE_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT           "CcRemoteDevice" )
  set ( CURRENT_PROJECT_IDE_PATH  "CcApps" )
  
  #############################################################################
  # Add source Files
  #############################################################################
  file (GLOB SOURCE_FILES
        "*.c"
        "*.cpp"
        "config/*.xml"
        "*.h")

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()
  
  #############################################################################
  # Add resources to this project
  #############################################################################
  find_program(CcOSResource_FOUND CcOSResource)
  set(CURRENT_PROJECT_RESOURCE_FILES
      ${CMAKE_CURRENT_LIST_DIR}/Resources/CcRemoteDevice.js
      ${CMAKE_CURRENT_LIST_DIR}/Resources/CcRemoteDevice.js.c
      ${CMAKE_CURRENT_LIST_DIR}/Resources/CcRemoteDevice.js.h
      ${CMAKE_CURRENT_LIST_DIR}/Resources/CcRemoteDevice.css
      ${CMAKE_CURRENT_LIST_DIR}/Resources/CcRemoteDevice.css.c
      ${CMAKE_CURRENT_LIST_DIR}/Resources/CcRemoteDevice.css.h)
   
  if(CcOSResource_FOUND)
      add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/Resources/CcRemoteDevice.js.c
                                ${CMAKE_CURRENT_LIST_DIR}/Resources/CcRemoteDevice.js.h
                                COMMAND CcOSResource -i CcRemoteDevice.js -o CcRemoteDevice.js -n CcRemoteDevice_Js -s
                                DEPENDS ${CMAKE_CURRENT_LIST_DIR}/Resources/CcRemoteDevice.js
                                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Resources)
      add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/Resources/CcRemoteDevice.css.c
                                ${CMAKE_CURRENT_LIST_DIR}/Resources/CcRemoteDevice.css.h
                                COMMAND CcOSResource -i CcRemoteDevice.css -o CcRemoteDevice.css -n CcRemoteDevice_Css -s
                                DEPENDS ${CMAKE_CURRENT_LIST_DIR}/Resources/CcRemoteDevice.css
                                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Resources)
      add_custom_target("${CURRENT_PROJECT}_Resources" DEPENDS  ${CURRENT_PROJECT_RESOURCE_FILES})
      set_target_properties("${CURRENT_PROJECT}_Resources" PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")                             
      source_group( "Resources" FILES ${CURRENT_PROJECT_RESOURCE_FILES})
  endif()
  
  #############################################################################
  # Setup project
  #############################################################################
  add_library( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} ${CURRENT_PROJECT_RESOURCE_FILES})
  
  #############################################################################
  # Setup project dependencies
  #############################################################################
  if(CcOSResource_FOUND)
    add_dependencies(${CURRENT_PROJECT} "${CURRENT_PROJECT}_Resources")
  endif()
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/Resources/${CURRENT_PROJECT_IDE_PATH}")
  
  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                  CcHttp
                                  CcDocuments)
  
  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} ${OPENSLL_INCLUDE_DIR} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
  
  ########
  # Add an unity-test, if available
  ########
  if( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
    add_subdirectory("test")
  endif()
endif()
