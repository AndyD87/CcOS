########
# Add Source Files
########
list (APPEND SOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Cpu.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Driver.cpp")

########
# Add Source Files
########
list (APPEND HEADER_FILES
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Cpu.h"
      "${CMAKE_CURRENT_LIST_DIR}/ESP8266Driver.h")
    
add_to_driver_headers("CPU/espressif/ESP8266/ESP8266Driver.h")
add_to_driver_names  (0 "ESP8266Driver")
add_to_driver_sources(${SOURCE_FILES} ${HEADER_FILES})
add_to_driver_includes(${CMAKE_CURRENT_SOURCE_DIR})

add_to_driver_definitions(${PROJECT_DEFINITIONS})
