################################################################################
# Check if Modules are required for Applications by reading config
################################################################################

# enable generic network stack
if( WINDOWS)
  add_subdirectory(Windows)
elseif( LINUX)
  add_subdirectory(Linux)
endif ()

push_up_driver_vars()
