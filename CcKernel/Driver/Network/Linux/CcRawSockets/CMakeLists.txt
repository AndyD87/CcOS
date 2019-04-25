if(LINUX)
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
        
  add_to_driver_headers("Network/Linux/CcRawSockets/CcRawSocketDriver.h")
  add_to_driver_names  (2 "CcRawSocketDriver")
  add_to_driver_sources(${HEADER_FILES} ${SOURCE_FILES})
  add_to_driver_includes(${CMAKE_CURRENT_LIST_DIR})
  add_to_driver_definitions()
else()
  message(WARNING "-- Linux network driver not supported")
endif()
