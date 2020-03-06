find_package( V4L QUIET)

if(NOT V4L_FOUND)
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
        "${CMAKE_CURRENT_LIST_DIR}/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/*h" )

  # If required for kernel load it now
  if(CCOS_CCKERNEL_DRIVER_CAMERA_LINUXV4L_ACTIVTE)
    add_to_driver_headers(Camera/LinuxV4L/CcV4LDriver.h)
    add_to_driver_names  (3 CcV4LDriver)
    add_to_driver_libraries(${V4L_LIBRARY})
    add_to_driver_sources(${SOURCE_FILES} ${HEADER_FILES})

  else()
    # Generate a loadable module
    set ( CURRENT_PROJECT           "CcModule.v4l" )
    set ( CURRENT_PROJECT_IDE_PATH  "CcKernel/Driver/Camera/LinuxV4L" )
    if(WINDOWS)
      CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
    endif()
    add_library( ${CURRENT_PROJECT} SHARED ${SOURCE_FILES})

    set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

    target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                    CcKernel)

    CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
    CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})

    CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)

  endif()
endif()
