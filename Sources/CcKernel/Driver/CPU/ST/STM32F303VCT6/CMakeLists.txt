########
# Add Source Files
########
list (APPEND SOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/../../Common/ARM_CM4/CcThreadData_ARM_CM4.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/../../Common/CortexMx/malloc_lock.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/startup_stm32f303xc.s"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F303VCT6Cpu.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F303VCT6Driver.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F303VCT6SystemGpioPort.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F303VCT6SystemGpioPin.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/../Common/STM32FX_IrqHandlers.cpp")

########
# Add Source Files
########
list (APPEND HEADER_FILES
      "${CMAKE_CURRENT_LIST_DIR}/stm32f3xx_hal_conf.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F303VCT6.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F303VCT6Cpu.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F303VCT6Driver.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F303VCT6SystemGpioPort.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F303VCT6SystemGpioPin.h")

list (APPEND PROJECT_DEFINITIONS
    -DSTM32F303xC -DHSE_VALUE=8000000)

set(STM32F3_DIR  ${CC_CACHE_DIR}/Sources/STM32F3Cube_Driver )

CcDownloadAndExtract( "STM32F3Cube_Driver"
                      "${STM32F3_DIR}"
                      "https://coolcow.de/projects/ThirdParty/STM32F3Cube/binaries/1.10.0/Drivers.7z")

# List all required source files
list (APPEND STM_SOURCE_FILES
      "${STM32F3_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c"
      "${STM32F3_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c"
      "${STM32F3_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c"
      "${STM32F3_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c"
      "${STM32F3_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c"
      "${STM32F3_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_wwdg.c"
      "${STM32F3_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c"
      "${STM32F3_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c"
      "${STM32F3_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_timebase_tim_template.c"
      "${STM32F3_DIR}/CMSIS/Device/ST/STM32F3xx/Source/Templates/system_stm32f3xx.c"
      )

if(CCOS_GENERIC_NETWORK)
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/STM32F303VCT6Network.cpp")
  list(APPEND HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/STM32F303VCT6Network.h")
  list(APPEND STM_SOURCE_FILES "${STM32F3_DIR}/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_eth.c")
  list(APPEND PROJECT_DEFINITIONS "-DCCOS_GENERIC_NETWORK")
endif(CCOS_GENERIC_NETWORK)

add_to_driver_headers("CPU/ST/STM32F303VCT6/STM32F303VCT6Driver.h")
add_to_driver_names  (0 "STM32F303VCT6Driver")
add_to_driver_sources(${STM_SOURCE_FILES} ${SOURCE_FILES} ${HEADER_FILES})
add_to_driver_includes(${STM32F3_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
                        "${STM32F3_DIR}/STM32F3xx_HAL_Driver/Inc"
                        "${STM32F3_DIR}/CMSIS/Include"
                        "${STM32F3_DIR}/CMSIS/Device/ST/STM32F3xx/Include")

add_to_driver_definitions(${PROJECT_DEFINITIONS})
