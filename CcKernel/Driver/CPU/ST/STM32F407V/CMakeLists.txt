########
# Add Source Files
########
list (APPEND SOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/../../Common/CortexMx/malloc_lock.c"
      "${CMAKE_CURRENT_LIST_DIR}/startup_stm32f407xx.s"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407VCpu.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407VDriver.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407VSystemGpioPin.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407VSystemGpioPort.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407VTimer.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/../STM32FX_IrqHandlers.cpp")

########
# Add Source Files
########
list (APPEND HEADER_FILES
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407V.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407VCpu.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407VDriver.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407VSystemGpioPin.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407VSystemGpioPort.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407VTimer.h"
      "${CMAKE_CURRENT_LIST_DIR}/stm32f4xx_hal_conf.h" )

list (APPEND PROJECT_DEFINITIONS
    "-DSTM32F407xx")

set(STM32F4_DIR  ${CC_CACHE_DIR}/ThirdParty/STM32F4Cube_Driver )

CcDownloadAndExtract( "STM32F4Cube_Driver"
                      "${STM32F4_DIR}"
                      "http://coolcow.de/projects/ThirdParty/STM32F4Cube/binaries/1.23.0/Drivers.7z")

# List all required source files
list (APPEND STM_SOURCE_FILES
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_tim_template.c"
      "${STM32F4_DIR}/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c"
      )

if(CCOS_DRIVER_NETWORK)
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/STM32F407VNetwork.cpp")
  list(APPEND HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/STM32F407VNetwork.h")
  list(APPEND STM_SOURCE_FILES "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c")
  list(APPEND PROJECT_DEFINITIONS "-DCCOS_DRIVER_NETWORK")
endif(CCOS_DRIVER_NETWORK)

add_to_driver_headers("CPU/ST/STM32F407V/STM32F407VDriver.h")
add_to_driver_names  (0 "STM32F407VDriver")
add_to_driver_sources(${STM_SOURCE_FILES} ${SOURCE_FILES} ${HEADER_FILES} ${COMMON_SOURCE_FILES})
add_to_driver_includes(${STM32F4_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
                        "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Inc"
                        "${STM32F4_DIR}/CMSIS/Include"
                        "${STM32F4_DIR}/CMSIS/Device/ST/STM32F4xx/Include")

add_to_driver_definitions(${PROJECT_DEFINITIONS})
