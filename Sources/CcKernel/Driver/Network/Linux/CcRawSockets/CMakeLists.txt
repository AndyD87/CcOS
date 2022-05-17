if(LINUX)
  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/CcRawSocketDriver.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRawSocketDriver.h
        ${CMAKE_CURRENT_LIST_DIR}/CcRawSocketNetwork.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRawSocketNetwork.h
  )
        
  add_to_driver_headers("Network/Linux/CcRawSockets/CcRawSocketDriver.h")
  add_to_driver_names  (2 "CcRawSocketDriver")
  add_to_driver_sources(${SOURCE_FILES})
  add_to_driver_includes(${CMAKE_CURRENT_LIST_DIR})
  add_to_driver_definitions()
else()
  message(WARNING "-- Linux network driver not supported")
endif()
