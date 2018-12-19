########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.cpp"
      "src/*.c" )

########
# Add Source Files
########
file (GLOB HEADER_FILES
      "*h" )

add_to_driver_headers("Board/ST/STM32F4Discovery/STM32F4DiscoveryDriver.h")
add_to_driver_names  (1 "STM32F4DiscoveryDriver")
add_to_driver_sources(${SOURCE_FILES} ${HEADER_FILES} ${STM_SOURCE_FILES})
add_to_driver_includes(${STM32F4_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
add_to_driver_definitions()
