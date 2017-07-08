###################
# Check if Modules are required for Applications by reading config
###################
if( CCOS_DRIVER_V4L )
  add_subdirectory(LinuxV4L)
endif()
if( CCOS_DRIVER_RASPICAM )
  add_subdirectory(RaspiCam)
endif()
if( CCOS_WINDOWS_DESKTOP_SCREEN )
  add_subdirectory(CcWindowsDesktopScreen)
endif()

push_up_driver_vars()
