if(NOT DEFINED CCOS_CCOSBUILDCONFIG_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCOSBUILDCONFIG_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT           CcOSBuildConfig )
  set ( CURRENT_PROJECT_IDE_PATH   )

  ########
  # Add Source Files
  ########
  file ( GLOB SOURCE_FILES
        "*.cpp"
        "*.h")

  set( LIBRARIES
        CcKernel
        CcDocuments )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )


  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
endif()
