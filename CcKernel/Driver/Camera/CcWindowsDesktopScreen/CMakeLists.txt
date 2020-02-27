if(WINDOWS)
  
  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.cpp"
        "*.c" )
  
  ########
  # Add Source Files
  ########
  file (GLOB HEADER_FILES
        "*h" )
  
  if(CCOS_WINDOWS_DESKTOP_SCREEN)
    add_to_driver_headers(Camera/CcWindowsDesktopScreen/CcWindowsDesktopScreenDriver.h)
    add_to_driver_names  (3 CcWindowsDesktopScreenDriver)
  endif()
  add_to_driver_sources(${SOURCE_FILES} ${HEADER_FILES})
endif()
