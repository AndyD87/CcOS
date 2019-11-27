################################################################################
# Check if Modules are required for Applications by reading config
################################################################################

# enable generic network stack
if( CCOS_DRIVER_WLAN_WINDOWS)
  add_subdirectory(Windows)
endif ()

push_up_driver_vars()
