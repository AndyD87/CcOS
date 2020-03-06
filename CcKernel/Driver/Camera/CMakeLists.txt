################################################################################
# Check if Modules are required for Applications by reading config
################################################################################
add_subdirectory(LinuxV4L)
add_subdirectory(CcWindowsDesktopScreen)
add_subdirectory(RaspiCam)
add_subdirectory(GPhoto)

push_up_driver_vars()
