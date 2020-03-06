if(WINDOWS)
  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.cpp"
        "*.c"
        "*.h" )
  
  if(CCOS_WINDOWS_DESKTOP_SCREEN)
    add_to_driver_sources(${SOURCE_FILES})
    add_to_driver_headers(Camera/CcWindowsDesktopScreen/CcWindowsDesktopScreenDriver.h)
    add_to_driver_names  (3 CcWindowsDesktopScreenDriver)
  else()
    set ( CURRENT_PROJECT           "CcModule.screen" )
    set ( CURRENT_PROJECT_IDE_PATH  "CcKernel/Driver/Camera/CcWindowsDesktopScreen" )

    add_definitions(${GPHOTO2_DEFINITIONS})

    include_directories( ${CMAKE_CURRENT_LIST_DIR} )

    if(WINDOWS)
      CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
    endif()

    add_library( ${CURRENT_PROJECT} SHARED ${SOURCE_FILES} )

    set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

    target_link_libraries ( ${CURRENT_PROJECT} CcKernel )

    add_to_driver_onload(Camera ${CURRENT_PROJECT})
  endif()
endif()
