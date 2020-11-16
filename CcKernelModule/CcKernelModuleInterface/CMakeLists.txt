if(NOT DEFINED CCOS_CCKERNELMODULE_INTERFACE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif( ${CCOS_CCKERNELMODULE_INTERFACE_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcKernelModuleInterface )
  set ( CURRENT_PROJECT_IDE_PATH  "CcKernelModule"    )

  #########
  ## Add Source Files
  #########
  file (GLOB SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
  )
  file (GLOB HEADER_FILES
        "${CMAKE_CURRENT_LIST_DIR}/*.h"
  )

  CcAddLibrary( ${CURRENT_PROJECT} ${SOURCE_FILES} ${HEADER_FILES})
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  set_target_properties(${CURRENT_PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${KERNEL_SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_LIST_DIR}/test)

endif()
