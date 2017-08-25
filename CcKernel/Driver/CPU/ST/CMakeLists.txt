################################################################################
# Check if Modules are required for Applications by reading config
################################################################################
if( CCOS_DRIVER_PROCESSOR_ST_STM32F407V)
  add_subdirectory(STM32F407V)
endif ()

push_up_driver_vars()
