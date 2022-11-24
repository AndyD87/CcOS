########
# Prepare environment
########

set(STM32F1_DIR           ${CC_CACHE_DIR}/Sources/STM32F1Cube_Driver )
set(STM32F1_DRIVERS_DIR   ${STM32F1_DIR}/Drivers)

CcGitClone( "${STM32F1_DIR}"
            "https://github.com/STMicroelectronics/STM32CubeF1"
            "blah")

########
# Add Source Files
########
list (APPEND SOURCE_FILES
      ${CMAKE_CURRENT_LIST_DIR}/../../Common/ARM_CM3/CcThreadData_ARM_CM3.cpp
      ${CMAKE_CURRENT_LIST_DIR}/../../Common/CortexMx/malloc_lock.cpp
      ${CMAKE_CURRENT_LIST_DIR}/startup_stm32f103x6.s
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103Cpu.cpp
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103Driver.cpp
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103I2C.cpp
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103SystemGpioPin.cpp
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103SystemGpioPort.cpp
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103Timer.cpp
      ${CMAKE_CURRENT_LIST_DIR}/../Common/STM32FX_IrqHandlers.cpp
)

########
# Add Source Files
########
list (APPEND HEADER_FILES
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103.h
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103Cpu.h
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103Driver.h
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103I2C.h
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103SystemGpioPin.h
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103SystemGpioPort.h
      ${CMAKE_CURRENT_LIST_DIR}/STM32F103Timer.h
      ${CMAKE_CURRENT_LIST_DIR}/stm32f1xx_hal_conf.h
)

########
# Add Include Directories
########
list (APPEND PROJECT_INCLUDE_DIRS
  "${CMAKE_CURRENT_LIST_DIR}/../Common"
  "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Inc"
  "${STM32F1_DRIVERS_DIR}/CMSIS/Include"
  "${STM32F1_DRIVERS_DIR}/CMSIS/Device/ST/STM32F1xx/Include"
)

########
# Add Definitions
########
list (APPEND PROJECT_DEFINITIONS
	-DSTM32F103xx
	-DUSE_HAL_DRIVER
)

if("${CCOS_CPU_TYPE}" STREQUAL "STM32F103C8")
  list(APPEND PROJECT_DEFINITIONS -DSTM32F103x6)
else()
  message(FATAL_ERROR "No driver for this CPU")
endif()

# List all required source files
list (APPEND STM_SOURCE_FILES
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_wwdg.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c"
      "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_timebase_tim_template.c"
      "${STM32F1_DRIVERS_DIR}/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c"
      )

########
# Add features
########
if(CCOS_GENERIC_NETWORK)
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/STM32F103Network.cpp")
  list(APPEND HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/STM32F103Network.h")
  list(APPEND STM_SOURCE_FILES "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/system_stm32f1xx_hal_eth.c")
  list(APPEND PROJECT_DEFINITIONS "-DCCOS_GENERIC_NETWORK")
endif(CCOS_GENERIC_NETWORK)

if(CCOS_GENERIC_USB)

  list(APPEND SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/usbh_conf.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/usbh_conf.h"
        "${CMAKE_CURRENT_LIST_DIR}/../Common/STM32Usb.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../Common/STM32Usb.h"
        "${CMAKE_CURRENT_LIST_DIR}/../Common/STM32UsbDevice.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../Common/STM32UsbDevice.h"
  )
  list(FILTER USB_SOURCE_FILES EXCLUDE REGEX "template")
  list(APPEND STM_SOURCE_FILES
        "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usb.c"
        "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_hcd.c"
        "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd.c"
        "${STM32F1_DRIVERS_DIR}/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd_ex.c"
        ${USB_SOURCE_FILES}
  )
  list(APPEND PROJECT_DEFINITIONS "-DCCOS_GENERIC_USB")
endif()

add_to_driver_headers("CPU/ST/STM32F103/STM32F103Driver.h")
add_to_driver_names  (0 "STM32F103Driver")
add_to_driver_sources(${SOURCE_FILES} ${STM_SOURCE_FILES} ${HEADER_FILES})
add_to_driver_includes(${STM32F1_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_INCLUDE_DIRS})

add_to_driver_definitions(${PROJECT_DEFINITIONS})
