
if( LINUX )
  find_package(Gphoto2 QUIET)
  if( GPHOTO2_FOUND )
    message("- GPHOTO2 found")
    set( GPHOTO2_FOUND       ${GPHOTO2_FOUND}       PARENT_SCOPE)
    set( GPHOTO2_LIB_DIR     ${GPHOTO2_LIB_DIR}     PARENT_SCOPE)
    set( GPHOTO2_LIBRARIES   ${GPHOTO2_LIBRARIES}   PARENT_SCOPE)

  elseif("${CCOS_THIRDPARTY_GPHOTO2_ACTIVE}" GREATER 2)
    message( FATAL_ERROR "- libgphoto2 marked as required, but not found on system. Please install libgphoto2-dev" )
  else()
    set(CCOS_THIRDPARTY_GPHOTO2_ACTIVE FALSE)
  endif()
endif()

if(GPHOTO2_FOUND)
  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
          ${CMAKE_CURRENT_LIST_DIR}/CcGphoto.h
          ${CMAKE_CURRENT_LIST_DIR}/CcGphotoCamera.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CcGphotoCamera.h
          ${CMAKE_CURRENT_LIST_DIR}/CcGphotoDriver.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CcGphotoDriver.h
          ${CMAKE_CURRENT_LIST_DIR}/CcGphotoModule.cpp
          ${CMAKE_CURRENT_LIST_DIR}/CcGphotoModule.h
  )

  # If required for kernel load it now
  if(CCOS_CCKERNEL_DRIVER_CAMERA_GPHOTO_ACTIVTE)
    #add_to_driver_headers(Camera/LinuxV4L/CcV4LDriver.h)
    #add_to_driver_names  (3 CcV4LDriver)
    #add_to_driver_libraries(${GPHOTO2_LIBRARIES} gphoto2_port)
    #add_to_driver_sources(${SOURCE_FILES})
  else()

    set ( CURRENT_PROJECT           "gphoto.ccm" )
    set ( CURRENT_PROJECT_IDE_PATH  "Driver/Camera/GPhoto" )

    add_definitions(${GPHOTO2_DEFINITIONS})

    include_directories( ${CMAKE_CURRENT_LIST_DIR} )

    if(WINDOWS)
      CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
    endif()

    CcAddLibrary( ${CURRENT_PROJECT} SHARED ${SOURCE_FILES} )

    set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

    target_link_libraries ( ${CURRENT_PROJECT}
                                  CcModule
                                  ${GPHOTO2_LIBRARIES}
                                  gphoto2_port
    )

    CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE)

    add_to_driver_onload(Camera ${CURRENT_PROJECT})
  endif()
endif()
