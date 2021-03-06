if(WINDOWS)
  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/WindowsWlan.h
        ${CMAKE_CURRENT_LIST_DIR}/WindowsWlanClient.cpp
        ${CMAKE_CURRENT_LIST_DIR}/WindowsWlanClient.h
        ${CMAKE_CURRENT_LIST_DIR}/WindowsWlanDriver.cpp
        ${CMAKE_CURRENT_LIST_DIR}/WindowsWlanDriver.h
        ${CMAKE_CURRENT_LIST_DIR}/WindowsWlanModule.cpp
        ${CMAKE_CURRENT_LIST_DIR}/WindowsWlanModule.h
  )

  if(CCOS_DRIVER_WLAN_WINDOWS)
    add_to_driver_sources(${SOURCE_FILES})
    add_to_driver_headers(Wlan/Windows/WindowsWlanDriver.h)
    add_to_driver_names  (3 WindowsWlanDriver)
    add_to_driver_libraries(wlanapi)
  else()  
    set ( CURRENT_PROJECT           "wlan.ccm" )
    set ( CURRENT_PROJECT_IDE_PATH  "Driver/Wlan/Windows" )

    include_directories( ${CMAKE_CURRENT_LIST_DIR} )

    if(WINDOWS)
      CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
    endif()

    CcAddLibrary( ${CURRENT_PROJECT} SHARED ${SOURCE_FILES} )

    set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

    target_link_libraries ( ${CURRENT_PROJECT} CcModule wlanapi )

    add_to_driver_onload(Wlan ${CURRENT_PROJECT})
    add_to_driver_onload(WlanClient ${CURRENT_PROJECT})
    add_to_driver_onload(WlanAccessPoint ${CURRENT_PROJECT})
  endif()
endif()
