if(NOT DEFINED CCOS_CCUTIL_CCTESTING_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCUTIL_CCTESTING_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcTesting )
  set ( CURRENT_PROJECT_IDE_PATH  CcUtil )

  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.cpp"
        "*.h"
  )

  set( LIBRARIES
        CcKernel
        CcModule)

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  # Make it always shared if not generic
  if(GENERIC)
    set(LIB_SHARED STATIC)
  else()
    set(LIB_SHARED SHARED)
  endif()
  CcAddLibrary( ${CURRENT_PROJECT}       ${LIB_SHARED} ${SOURCE_FILES} )
  CcAddLibrary( "${CURRENT_PROJECT}4Test"  ${LIB_SHARED} ${SOURCE_FILES} )
  
  set_property( TARGET ${CURRENT_PROJECT}      PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  set_property( TARGET ${CURRENT_PROJECT}4Test PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT}      LINK_PRIVATE ${LIBRARIES} )
  target_link_libraries ( ${CURRENT_PROJECT}4Test LINK_PRIVATE ${LIBRARIES} )
  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT}4Test )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
  CcOSLibSettings(${CURRENT_PROJECT}4Test TRUE TRUE ${SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()
