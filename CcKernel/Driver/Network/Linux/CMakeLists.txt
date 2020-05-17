################################################################################
# Check if Modules are required for Applications by reading config
################################################################################

if( CCOS_DRIVER_NETWORK_LINUX_CCRAWSOCKET)
  add_subdirectory(CcRawSockets)
endif ()

add_subdirectory(DBus)

push_up_driver_vars()
