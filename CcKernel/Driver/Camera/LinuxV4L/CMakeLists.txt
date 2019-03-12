find_package( libv4l QUIET)
set( DoBuildPackage FALSE)
if(libv4l_FOUND)
    set( DoBuildPackage TRUE)
else()
    set( DoBuildPackage FALSE)
    if( "${CCOS_DRIVER_V4L}" STREQUAL "r")
      message( FATAL_ERROR "v4l marked as required, but not found on system. Please install libv4l-dev" )
    else()
      unset(CCOS_DRIVER_V4L)
    endif()
endif()

if(DoBuildPackage EQUAL TRUE)
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


    include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
                         
    add_library(driverLinuxV4L ${CC_LINK_TYPE} ${SOURCE_FILES} ${HEADER_FILES} )

    link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../../lib )

    set_property( TARGET driverLinuxV4L PROPERTY FOLDER "CcOS")

    target_link_libraries ( driverLinuxV4L LINK_PRIVATE
                                  CcKernel
                                  libv4l-0 )

    set_property( TARGET CcMedia PROPERTY FOLDER "CcOS/IDriver/Camera")

    target_include_directories( driverLinuxV4L PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif()
