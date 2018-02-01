if(NOT DEFINED CCOS_CCNETWORK_CCSSL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCNETWORK_CCSSL_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcSsl )
  
  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "src/*.c"
        "src/*.cpp"
        "config/*.xml"
        "*.h")
   
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT})
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
  endif()
   
  add_library( ${CURRENT_PROJECT} ${CCOS_LINK_TYPE} ${SOURCE_FILES})
  
  if(WINDOWS)
    add_dependencies( ${CURRENT_PROJECT} openssl )
  endif()
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcNetwork")
  
  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                                  CcKernel
                                  ${OPENSSL_LIBRARIES} )
   
  
  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} ${OPENSLL_INCLUDE_DIR} )
  CcOSSetInstallConfig ( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
endif()