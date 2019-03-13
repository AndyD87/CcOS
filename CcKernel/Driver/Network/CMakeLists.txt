################################################################################
# Check if Modules are required for Applications by reading config
################################################################################
if( CCOS_DRIVER_NETWORK_DP83848)
  add_subdirectory(DP83848)
endif ()

push_up_driver_vars()
