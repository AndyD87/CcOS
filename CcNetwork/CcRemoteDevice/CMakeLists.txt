if(NOT DEFINED CCOS_CCNETWORK_CCREMOTEDEVICE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCNETWORK_CCREMOTEDEVICE_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcRemoteDevice )
  
  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.c"
        "*.cpp"
        "config/*.xml"
        "*.h")
   
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()
   
  add_library( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES})
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcNetwork")
  
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
