if(WINDOWS)
  ########
  # Add Source Files
  ########
  set( SOURCE_FILESs
          ${CMAKE_CURRENT_LIST_DIR}/DirectShowDevice.cpp
          ${CMAKE_CURRENT_LIST_DIR}/DirectShowDevice.h
          ${CMAKE_CURRENT_LIST_DIR}/DirectShowDriver.cpp
          ${CMAKE_CURRENT_LIST_DIR}/DirectShowDriver.h
          ${CMAKE_CURRENT_LIST_DIR}/DirectShowModule.cpp
          ${CMAKE_CURRENT_LIST_DIR}/DirectShowModule.h
  )

  if(CCOS_WINDOWS_DESKTOP_SCREEN)
    add_to_driver_sources   (${SOURCE_FILES})
    add_to_driver_headers   (Camera/DirectShow/DirectShowDriver.h)
    add_to_driver_names     (3 DirectShowDriver)
    add_to_driver_libraries (3 DirectShowDriver)
  else()
    set ( CURRENT_PROJECT           "directshow.ccm" )
    set ( CURRENT_PROJECT_IDE_PATH  "Driver/Camera/DirectShow" )

    include_directories( ${CMAKE_CURRENT_LIST_DIR} )

    if(WINDOWS)
      CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
    endif()

    CcAddLibrary( ${CURRENT_PROJECT} SHARED ${SOURCE_FILES} )

    set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

    target_link_libraries ( ${CURRENT_PROJECT} CcModule
       
    )

    add_to_driver_onload(Camera ${CURRENT_PROJECT})
  endif()
endif()
