################################################################################
# Check if Modules are required for Applications by reading config
################################################################################

add_subdirectory(ST)

if(DEFINED CCOS_BOARD_TYPE)
  CcStringStartsWith(TYPE_START ${CCOS_BOARD_TYPE} "ORANGEPI")
  if(${TYPE_START})
    add_subdirectory(OrangePi)
  endif()
endif()
  
push_up_driver_vars()
