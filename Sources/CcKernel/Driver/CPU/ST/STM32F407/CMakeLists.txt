########
# Prepare environment
########

set(STM32F4_VERSION           "1.27.0" )
set(STM32F4_DIR               ${CC_CACHE_DIR}/Sources/STM32F4Cube_Driver/${STM32F4_VERSION} )
set(STM32F4_DRIVER_DIR        ${STM32F4_DIR}/Drivers )

CcDownloadAndExtract( "STM32F4Cube_Driver"
"${STM32F4_DIR}"
"https://coolcow.de/projects/ThirdParty/STM32F4Cube/binaries/${STM32F4_VERSION}/Drivers.7z")

########
# Add Source Files
########
list (APPEND SOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/../../Common/ARM_CM4/CcThreadData_ARM_CM4.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/../../Common/CortexMx/malloc_lock.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/startup_stm32f407xx.s"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407Cpu.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407Driver.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407SystemGpioPin.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407SystemGpioPort.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407Timer.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/../Common/STM32FX_IrqHandlers.cpp"
)

########
# Add Header Files
########
list (APPEND HEADER_FILES
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407Cpu.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407Driver.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407SystemGpioPin.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407SystemGpioPort.h"
      "${CMAKE_CURRENT_LIST_DIR}/STM32F407Timer.h"
      "${CMAKE_CURRENT_LIST_DIR}/stm32f4xx_hal_conf.h"
)

########
# Add Include Directories
########
list (APPEND PROJECT_INCLUDE_DIRS
  "${CMAKE_CURRENT_LIST_DIR}/../Common"
  "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Inc"
  "${STM32F4_DRIVER_DIR}/CMSIS/Include"
  "${STM32F4_DRIVER_DIR}/CMSIS/Device/ST/STM32F4xx/Include"
)

########
# Add Definitions
########
list (APPEND PROJECT_DEFINITIONS
    -DARM_MATH_CM4
    -DSTM32F407xx
    -DHSE_VALUE=8000000
)

if("${CCOS_CPU_TYPE}" STREQUAL "STM32F407VET")
  list(APPEND PROJECT_DEFINITIONS -DNUMBER_OF_PORTS=9 -DSTM32F407VET)
elseif("${CCOS_CPU_TYPE}" STREQUAL "STM32F407VGT")
  list(APPEND PROJECT_DEFINITIONS -DNUMBER_OF_PORTS=9 -DSTM32F407VGT)
else()
  message(FATAL_ERROR "No driver for this CPU")
endif()

# List all required source files
list (APPEND STM_SOURCE_FILES
      "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c"
      "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c"
      "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c"
      "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c"
      "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c"
      "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c"
      "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c"
      "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c"
      "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_tim_template.c"
      "${STM32F4_DRIVER_DIR}/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c"
      )

########
# Add features
########
if(CCOS_GENERIC_NETWORK)
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/STM32F407Network.cpp")
  list(APPEND HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/STM32F407Network.h")
  list(APPEND STM_SOURCE_FILES  "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c")
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
        "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c"
        "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c"
        "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c"
        "${STM32F4_DRIVER_DIR}/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c"
        ${USB_SOURCE_FILES}
  )
  list(APPEND PROJECT_DEFINITIONS "-DCCOS_GENERIC_USB")
endif()

add_to_driver_headers("CPU/ST/STM32F407/STM32F407Driver.h")
add_to_driver_names  (0 "STM32F407Driver")
add_to_driver_sources(${SOURCE_FILES} ${STM_SOURCE_FILES} ${HEADER_FILES})
add_to_driver_includes(${STM32F4_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_INCLUDE_DIRS})
add_to_driver_definitions(${PROJECT_DEFINITIONS})
