########
# Add Source Files
########
list (APPEND SOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/../../Common/CortexMx/malloc_lock.c"
      "${CMAKE_CURRENT_LIST_DIR}/startup_STM32F103xx.s"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F103Cpu.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F103Driver.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F103SystemGpioPin.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F103SystemGpioPort.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F103Timer.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/../STM32FX_IrqHandlers.cpp")


########
# Add Source Files
########
list (APPEND HEADER_FILES
      "${CMAKE_CURRENT_LIST_DIR}/STM32F103.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F103Cpu.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F103Driver.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F103SystemGpioPin.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F103SystemGpioPort.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F103Timer.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F1xx_hal_conf.h" )

list (APPEND PROJECT_DEFINITIONS
    "-DSTM32F103xx")

set(STM32F1_DIR           ${CC_CACHE_DIR}/ThirdParty/STM32F1Cube_Driver )
set(STM32F1_DRIVERS_DIR   ${STM32F1_DIR}/Drivers)

CcGitClone( "${STM32F1_DIR}"
            "https://github.com/STMicroelectronics/STM32CubeF1")

# List all required source files
list (APPEND STM_SOURCE_FILES
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/STM32F1xx_hal.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/STM32F1xx_hal_gpio.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/STM32F1xx_hal_uart.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/STM32F1xx_hal_rcc.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/STM32F1xx_hal_cortex.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/STM32F1xx_hal_wwdg.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/STM32F1xx_hal_tim.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/STM32F1xx_hal_tim_ex.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/STM32F1xx_hal_timebase_tim_template.c"
      "${STM32F1_DRIVERS_DIR}/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_STM32F1xx.c"
      )

if(CCOS_DRIVER_NETWORK)
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/STM32F103Network.cpp")
  list(APPEND HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/STM32F103Network.h")
  list(APPEND STM_SOURCE_FILES "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/STM32F1xx_hal_eth.c")
  list(APPEND PROJECT_DEFINITIONS "-DCCOS_DRIVER_NETWORK")
endif(CCOS_DRIVER_NETWORK)

add_to_driver_headers("CPU/ST/STM32F103/STM32F103Driver.h")
add_to_driver_names  (0 "STM32F103Driver")
add_to_driver_sources(${STM_SOURCE_FILES} ${SOURCE_FILES} ${HEADER_FILES} ${COMMON_SOURCE_FILES})
add_to_driver_includes(${STM32F1_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
                        "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Inc"
                        "${STM32F1_DRIVERS_DIR}/CMSIS/Include"
                        "${STM32F1_DRIVERS_DIR}/CMSIS/Device/ST/STM32F1xx/Include")

add_to_driver_definitions(${PROJECT_DEFINITIONS})
