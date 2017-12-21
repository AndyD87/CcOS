if(NOT DEFINED CCOS_CCUTIL_CCOSBUILDCONFIG_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCUTIL_CCOSBUILDCONFIG_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcOSBuildConfig )
  ########
  # Add Source Files
  ########
  file ( GLOB SOURCE_FILES
        "src/*.cpp"
        "*.h")
  
  set( LIBRARIES
        CcKernel
        CcDocuments )
  
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT})
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
  endif()
  
  add_library( ${CURRENT_PROJECT} ${CCOS_LINK_TYPE} ${SOURCE_FILES} )
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcUtil")
  
  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )
  
      
  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
endif()