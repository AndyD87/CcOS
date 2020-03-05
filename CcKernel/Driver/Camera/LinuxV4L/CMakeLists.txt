find_package( V4L QUIET)

if(NOT V4L_FOUND)
  message( FATAL_ERROR "${HEADER_FILES}" )
  if( "${CCOS_DRIVER_V4L}" STREQUAL "r")
    message( FATAL_ERROR "v4l marked as required, but not found on system. Please install libv4l-dev" )
  endif()
endif()

if(V4L_FOUND)
  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/*.c" )

  ########
  # Add Source Files
  ########
  file (GLOB HEADER_FILES
        "${CMAKE_CURRENT_LIST_DIR}/*h" )

  add_to_driver_headers(Camera/LinuxV4L/CcV4LDriver.h)
  add_to_driver_names  (3 CcV4LDriver)
  add_to_driver_libraries(${V4L_LIBRARY})
  add_to_driver_sources(${SOURCE_FILES} ${HEADER_FILES})
endif()
