########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.cpp"
      "*.c" )

########
# Add Source Files
########
file (GLOB HEADER_FILES
      "*h" )
      
set(STM32F4_DIR  ${CC_CACHE_DIR}/ThirdParty/STM32F4Cube_Driver )

CcDownloadAndExtract( "STM32F4Cube_Driver" 
                      "${STM32F4_DIR}"
                      "http://coolcow.de/projects/ThirdParty/STM32F4Cube/binaries/1.23.0/Drivers.7z")

# List all required source files
list (APPEND STM_SOURCE_FILES 
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c"
      "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_tim_template.c"
      "${STM32F4_DIR}/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.s"
      "${STM32F4_DIR}/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c"
      )

add_to_driver_headers("CPU/ST/STM32F407V/STM32F407VDriver.h")
add_to_driver_names  (0 "STM32F407VDriver")
add_to_driver_sources(${SOURCE_FILES} ${HEADER_FILES} ${STM_SOURCE_FILES})
add_to_driver_includes(${STM32F4_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} 
                        "${STM32F4_DIR}/STM32F4xx_HAL_Driver/Inc" 
                        "${STM32F4_DIR}/CMSIS/Include"
                        "${STM32F4_DIR}/CMSIS/Device/ST/STM32F4xx/Include")

add_to_driver_definitions(-DSTM32F407xx)
