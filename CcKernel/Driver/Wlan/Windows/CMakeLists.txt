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
  
  add_to_driver_headers("Wlan/Windows/WindowsWlanDriver.h")
  ## Do not add to auto load, may be in later implementaions per define
  #  add_to_driver_names  (2 "WindowsWlanDriver")
  add_to_driver_sources(${HEADER_FILES} ${SOURCE_FILES})
  add_to_driver_includes()
  add_to_driver_definitions()
  add_to_driver_libraries(Wlanapi)
else()
  message(WARNING "-- Windows wlan driver not supported")
endif()