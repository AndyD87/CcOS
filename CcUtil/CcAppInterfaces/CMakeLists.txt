if(NOT DEFINED CCOS_CCUTIL_CCAPPINTERFACES_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCUTIL_CCAPPINTERFACES_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcAppInterfaces )
  set ( CURRENT_PROJECT_IDE_PATH  CcUtil )

  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.h"
        "CcVBoxIfc/*.cpp"
        "CcVBoxIfc/*.h"
  )

  set( LIBRARIES
        CcKernel
  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  add_library( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE ${LIBRARIES} )

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
