########
# Linker script for own raw sections in flash
########
set(CCOS_ESP8266_LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/ESP8266_CcOS.ld")

########
# Add Source Files
########
list (APPEND SOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/../../Common/FreeRTOS/FreeRTOSCpu.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Clk.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Cpu.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Eeprom.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266GpioPort.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Spi.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Timer.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Wlan.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266WlanAccessPoint.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266WlanClient.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Driver.cpp"
      ${CCOS_ESP8266_LINKER_SCRIPT})

########
# Add Source Files
########
list (APPEND HEADER_FILES
      "${CMAKE_CURRENT_LIST_DIR}/../../Common/FreeRTOS/FreeRTOSCpu.h"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Clk.h"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Cpu.h"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Eeprom.h"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266GpioPort.h"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Spi.h"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Timer.h"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Wlan.h"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266WlanAccessPoint.h"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266WlanClient.h"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Driver.h")

add_to_driver_headers("CPU/espressif/ESP8266/ESP8266Driver.h")
add_to_driver_names  (0 "ESP8266Driver")
add_to_driver_sources(${SOURCE_FILES} ${HEADER_FILES})
add_to_driver_includes(
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config
  ${ESP8266_RTOS_SDK_DIR}/components/esp8266/include
)
#add_to_driver_dependencies(__idf_freertos )
add_to_driver_libraries(freertos heap lwip tcpip_adapter nvs_flash)
add_to_driver_definitions(${PROJECT_DEFINITIONS})

# enable lwip network stack
set(CCOS_CCKERNEL_GENERIC_LWIP_STACK TRUE CACHE INTERNAL "")
set(CCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER TRUE CACHE INTERNAL "")

###############################################################################
# Import the litte modified debug library from
#   https://github.com/espressif/esp-gdbstub.git 
#     Commit: c86fefef41ec2b9f192e600760c2145a8382e803
###############################################################################
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  list(APPEND GDB_FILES
    gdbstub/gdbstub.c
    gdbstub/gdbstub.h
    gdbstub/gdbstub-cfg.h
    gdbstub/gdbstub-entry.h
    gdbstub/gdbstub-entry.S
  )
    
  CcAddLibrary(gdbstub STATIC ${GDB_FILES})
  target_link_libraries(gdbstub esp8266)
endif()

add_to_kernel_scripts(${CCOS_ESP8266_LINKER_SCRIPT})
