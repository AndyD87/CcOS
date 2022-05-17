if(NOT DEFINED CCOS_MODULES_CTESTMODULE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(NOT KERNELHEADERS_FOUND)
elseif(${CCOS_MODULES_CCTESTMODULE_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           "CcTestModule" )
  set ( CURRENT_PROJECT_IDE_PATH  "CcModules" )

  #############################################################################
  # Add source Files
  #############################################################################
  file (GLOB SOURCE_FILES
        "*.c"
        "*.cpp"
        "*.h"
  )
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  #############################################################################
  # Setup project
  #############################################################################
  CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} ${CURRENT_PROJECT_RESOURCE_FILES})

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                  CcKernel
  )

  #################
  # Post configuration steps
  #################
  CcTargetIncludes( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
