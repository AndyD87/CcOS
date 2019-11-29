if(NOT DEFINED CCOS_CCMEDIA_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCMEDIA_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcMedia )

  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "CcImage.cpp"
        "CcVideo.cpp"
        "CcImage.h"
        "CcImageAbstract.h"
        "CcVideo.h"
  )

  set( LIBRARIES
          CcKernel )


  if( CCOS_THIRDPARTY_LIBJPG )
    list(APPEND SOURCE_FILES "CcLibjpeg.h" )
    list(APPEND SOURCE_FILES "CcLibjpeg.cpp" )
    list(APPEND LIBRARIES jpeg)
  endif()

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  add_library(${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
endif()
