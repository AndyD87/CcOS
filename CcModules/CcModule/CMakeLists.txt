if(NOT DEFINED CCOS_CCMODULES_CCMODULE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCMODULES_CCMODULE_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcModule )
  set ( CURRENT_PROJECT_IDE_PATH  CcModules )

  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.c"
        "*.cpp"
        "config/*.xml"
        "*.h"
  )

  set(CcModule_LIBRARIES CcKernel)
  set(CcModule_DEFINITIONS )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  add_definitions(${CcModule_DEFINITIONS})
  CcAddLibrary( ${CURRENT_PROJECT} STATIC ${SOURCE_FILES})

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                ${CcModule_LIBRARIES} )

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
