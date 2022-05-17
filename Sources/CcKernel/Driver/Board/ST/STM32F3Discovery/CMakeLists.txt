########
# Add Source Files
########
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/STM32F3Discovery.h
        ${CMAKE_CURRENT_LIST_DIR}/STM32F3DiscoveryDriver.cpp
        ${CMAKE_CURRENT_LIST_DIR}/STM32F3DiscoveryDriver.h
        ${CMAKE_CURRENT_LIST_DIR}/STM32F3DiscoveryLed.cpp
        ${CMAKE_CURRENT_LIST_DIR}/STM32F3DiscoveryLed.h
)

add_to_driver_headers("Board/ST/STM32F4Discovery/STM32F3DiscoveryDriver.h")
add_to_driver_names  (1 "STM32F3DiscoveryDriver")
add_to_driver_sources(${SOURCE_FILES} ${STM_SOURCE_FILES})
add_to_driver_includes(${STM32F3_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
add_to_driver_definitions()
