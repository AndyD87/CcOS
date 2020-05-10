########
# Add Source Files
########
list (APPEND SOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/../../Common/CortexMx/malloc_lock.c"
      "${CMAKE_CURRENT_LIST_DIR}/startup_stm32f207xx.s"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F207IGCpu.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F207IGDriver.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F207IGSystemGpioPort.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F207IGSystemGpioPin.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/../STM32FX_IrqHandlers.cpp")

########
# Add Source Files
########
list (APPEND HEADER_FILES
      "${CMAKE_CURRENT_LIST_DIR}/stm32f2xx_hal_conf.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F207IG.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F207IGCpu.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F207IGDriver.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F207IGSystemGpioPort.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F207IGSystemGpioPin.h")

list (APPEND PROJECT_DEFINITIONS
    "-DSTM32F207xx")

set(STM32F2_DIR  ${CC_CACHE_DIR}/ThirdParty/STM32F2Cube_Driver )

CcDownloadAndExtract( "STM32F2Cube_Driver"
                      "${STM32F2_DIR}"
                      "http://coolcow.de/projects/ThirdParty/STM32F2Cube/binaries/1.7.0/Drivers.7z")

# List all required source files
list (APPEND STM_SOURCE_FILES
      "${STM32F2_DIR}/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal.c"
      "${STM32F2_DIR}/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_gpio.c"
      "${STM32F2_DIR}/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_uart.c"
      "${STM32F2_DIR}/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rcc.c"
      "${STM32F2_DIR}/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_cortex.c"
      "${STM32F2_DIR}/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_wwdg.c"
      "${STM32F2_DIR}/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_tim.c"
      "${STM32F2_DIR}/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_tim_ex.c"
      "${STM32F2_DIR}/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_timebase_tim_template.c"
      "${STM32F2_DIR}/CMSIS/Device/ST/STM32F2xx/Source/Templates/system_stm32f2xx.c"
      )

if(CCOS_DRIVER_NETWORK)
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/STM32F207IGNetwork.cpp")
  list(APPEND HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/STM32F207IGNetwork.h")
  list(APPEND STM_SOURCE_FILES "${STM32F2_DIR}/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_eth.c")
  list(APPEND PROJECT_DEFINITIONS "-DCCOS_DRIVER_NETWORK")
endif(CCOS_DRIVER_NETWORK)

add_to_driver_headers("CPU/ST/STM32F207IG/STM32F207IGDriver.h")
add_to_driver_names  (0 "STM32F207IGDriver")
add_to_driver_sources(${STM_SOURCE_FILES} ${SOURCE_FILES} ${HEADER_FILES} ${COMMON_SOURCE_FILES})
add_to_driver_includes(${STM32F2_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
                        "${STM32F2_DIR}/STM32F2xx_HAL_Driver/Inc"
                        "${STM32F2_DIR}/CMSIS/Include"
                        "${STM32F2_DIR}/CMSIS/Device/ST/STM32F2xx/Include")

add_to_driver_definitions(${PROJECT_DEFINITIONS})
