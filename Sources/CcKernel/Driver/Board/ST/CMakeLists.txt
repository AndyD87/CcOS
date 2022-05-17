################################################################################
# Check if Modules are required for Applications by reading config
################################################################################
if( CCOS_BOARD_TYPE)
  if(${CCOS_BOARD_TYPE} STREQUAL "STM32F4DISCOVERY")
  	add_subdirectory(STM32F4Discovery)
  elseif(${CCOS_BOARD_TYPE}  STREQUAL "STM32F407VET")
  	add_subdirectory(STM32F407VET)
  endif()
elseif( CCOS_DRIVER_BOARD_ST_STM3220GEVAL)
  add_subdirectory(STM3220GEVAL)
endif ()

push_up_driver_vars()
