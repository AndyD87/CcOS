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
    set ( CURRENT_PROJECT           "screen.ccm" )
    set ( CURRENT_PROJECT_IDE_PATH  "Driver/Camera/CcWindowsDesktopScreen" )

    include_directories( ${CMAKE_CURRENT_LIST_DIR} )

    if(WINDOWS)
      CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
    endif()

    CcAddLibrary( ${CURRENT_PROJECT} SHARED ${SOURCE_FILES} )

    set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

    target_link_libraries ( ${CURRENT_PROJECT} CcKernel )

    add_to_driver_onload(Camera ${CURRENT_PROJECT})
  endif()
endif()
