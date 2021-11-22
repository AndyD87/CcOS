################################################################################
# Check if Modules are required for Applications by reading config
################################################################################

if(DEFINED CCOS_BOARD_TYPE)
  if(${CCOS_BOARD_TYPE} STREQUAL "ORANGEPI_ZERO")
    add_subdirectory(Zero)
  endif()
endif()
  
push_up_driver_vars()
