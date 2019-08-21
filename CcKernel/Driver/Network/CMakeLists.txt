################################################################################
# Check if Modules are required for Applications by reading config
################################################################################

# enable generic network stack
set(CCOS_CCKERNEL_GENERIC_NETWORK_STACK TRUE CACHE INTERNAL "")

if( CCOS_DRIVER_NETWORK_WINDOWS)
  add_subdirectory(Windows)
endif ()

if( CCOS_DRIVER_NETWORK_LINUX)
  add_subdirectory(Linux)
endif ()
push_up_driver_vars()
