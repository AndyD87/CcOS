################################################################################
# Check if Modules are required for Applications by reading config
################################################################################

if( CCOS_DRIVER_NETWORK_WINDOWS_CCRAWNDIS)
  add_subdirectory(CcRawNdis)
endif ()

push_up_driver_vars()
