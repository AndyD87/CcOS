
if( LINUX )
  find_package(Gphoto2 QUIET)
  if( GPHOTO2_FOUND )
    message("- GPHOTO2 found")
    set( GPHOTO2_FOUND       ${GPHOTO2_FOUND}       PARENT_SCOPE)
    set( GPHOTO2_LIB_DIR     ${GPHOTO2_LIB_DIR}     PARENT_SCOPE)
    set( GPHOTO2_LIBRARIES   ${GPHOTO2_LIBRARIES}   PARENT_SCOPE)

  elseif(${CCOS_THIRDPARTY_GPHOTO2_ACTIVE} GREATER 2)
    message( FATAL_ERROR "- libgphoto2 marked as required, but not found on system. Please install libgphoto2-dev" )
  else()
    set(CCOS_THIRDPARTY_GPHOTO2_ACTIVE FALSE)
  endif()
endif()

if(GPHOTO2_FOUND)
  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.c"
        "*.cpp"
        "config/*.xml"
        "*.h"
  )

  # If required for kernel load it now
  if(CCOS_CCKERNEL_DRIVER_CAMERA_GPHOTO_ACTIVTE)
    #add_to_driver_headers(Camera/LinuxV4L/CcV4LDriver.h)
    #add_to_driver_names  (3 CcV4LDriver)
    #add_to_driver_libraries(${GPHOTO2_LIBRARIES} gphoto2_port)
    #add_to_driver_sources(${SOURCE_FILES} ${HEADER_FILES})
  else()

    set ( CURRENT_PROJECT           "CcModule.gphoto" )
    set ( CURRENT_PROJECT_IDE_PATH  "CcKernel/Driver/Camera/GPhoto" )

    add_definitions(${GPHOTO2_DEFINITIONS})

    include_directories( ${CMAKE_CURRENT_LIST_DIR} )

    if(WINDOWS)
      CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
    endif()

    add_library( ${CURRENT_PROJECT} SHARED ${SOURCE_FILES} )

    set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

    target_link_libraries ( ${CURRENT_PROJECT}
                                  CcKernel
                                  ${GPHOTO2_LIBRARIES}
                                  gphoto2_port
    )
  endif()
endif()
