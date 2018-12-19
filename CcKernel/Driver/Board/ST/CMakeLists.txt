################################################################################
# Check if Modules are required for Applications by reading config
################################################################################
if( CCOS_DRIVER_BOARD_ST_STM32F4DISCOVERY)
  add_subdirectory(STM32F4Discovery)
endif ()

push_up_driver_vars()
