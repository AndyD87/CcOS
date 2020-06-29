################################################################################
# Check if Modules are required for Applications by reading config
################################################################################
if("${CCOS_CPU_DRIVER}" STREQUAL "STM32F103")
  add_subdirectory(STM32F103)
elseif("${CCOS_CPU_DRIVER}" STREQUAL "STM32F407")
  add_subdirectory(STM32F407)
elseif(CCOS_DRIVER_CPU_ST_STM32F207IG)
  add_subdirectory(STM32F207IG)
elseif(CCOS_DRIVER_CPU_ST_STM32F303VCT6)
  add_subdirectory(STM32F303VCT6)
endif ()

push_up_driver_vars()
