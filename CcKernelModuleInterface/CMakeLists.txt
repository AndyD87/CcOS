if(NOT DEFINED CCOS_CCKERNELMODULEINTERFACE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif( ${CCOS_CCKERNELMODULEINTERFACE_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcKernelModuleInterface )
  set ( CURRENT_PROJECT_IDE_PATH                    )

  #########
  ## Add Source Files
  #########
  set ( HEADER_FILES
        CcKernelModuleInterface.h
  )

  set ( SOURCE_FILES
        CcKernelModuleInterface.cpp
  )
  set( LIBRARIES
        CcKernel
  )

  CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} ${HEADER_FILES})

  set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  CcTargetHeaders( ${CURRENT_PROJECT} "${HEADER_FILES}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )

  #################
  # Post configuration steps
  #################
  CcTargetIncludes( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${HEADER_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_LIST_DIR}/test)

endif()
