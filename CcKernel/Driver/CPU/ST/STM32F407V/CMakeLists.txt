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

add_to_driver_headers("CPU/ST/STM32F407V/STM32F407VDriver.h")
add_to_driver_names  ("STM32F407VDriver")
add_to_driver_sources(${SOURCE_FILES} ${HEADER_FILES})
