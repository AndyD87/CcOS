########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.c"
      "*.cpp"
      "*.h")

add_to_kernel_sources    ( ${SOURCE_FILES} )
add_to_kernel_definitions( ${DEFINITIONS} )
add_to_kernel_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} )
add_to_kernel_libraries  ( pthread rt dl
                           crypt )
