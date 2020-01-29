if(NOT DEFINED CCOS_CCUTIL_CCGPHOTO_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCUTIL_CCGPHOTO_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcGphoto )
  set ( CURRENT_PROJECT_IDE_PATH  CcUtil )

  if(GPHOTO2_FOUND)
    ########
    # Add Source Files
    ########
    file (GLOB SOURCE_FILES
          "*.c"
          "*.cpp"
          "config/*.xml"
          "*.h"
    )

    add_definitions(${GPHOTO2_DEFINITIONS})

    include_directories( ${CMAKE_CURRENT_LIST_DIR} )

    if(WINDOWS)
      CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
    endif()

    add_library( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

    set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

    target_link_libraries ( ${CURRENT_PROJECT} PUBLIC
                                  CcKernel
                                  CcMedia
                                  ${GPHOTO2_LIBRARIES} )

    #################
    # Post configuration steps
    #################
    CcOSTargetIncludeDirs( ${CURRENT_PROJECT} ${GPHOTO2_INCLUDE_DIR})
    CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
  endif()
endif()
