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

add_to_driver_headers("Network/DP83848/DP83848Driver.h")
add_to_driver_names  (2 "DP83848Driver")
add_to_driver_sources(${SOURCE_FILES} ${HEADER_FILES})
add_to_driver_includes(${CMAKE_CURRENT_LIST_DIR})
