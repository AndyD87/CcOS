################################################################################
# Check if Modules are required for Applications by reading config
################################################################################
if( CCOS_DRIVER_CPU_ST)
  add_subdirectory(ST)
endif ()

push_up_driver_vars()
