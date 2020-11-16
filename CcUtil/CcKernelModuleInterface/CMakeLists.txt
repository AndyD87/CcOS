if(NOT DEFINED CCOS_CCUTIL_CCKERNELMODULEINTERFACE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif( ${CCOS_CCUTIL_CCKERNELMODULEINTERFACE_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcKernelModuleInterface )
  set ( CURRENT_PROJECT_IDE_PATH  CcUtil                  )

  #########
  ## Add Source Files
  #########
  file (GLOB SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
  )
  file (GLOB HEADER_FILES
        "${CMAKE_CURRENT_LIST_DIR}/*.h"
  )

  set( LIBRARIES
        CcKernel
  )

  CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} ${HEADER_FILES})
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${HEADER_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_LIST_DIR}/test)

endif()
