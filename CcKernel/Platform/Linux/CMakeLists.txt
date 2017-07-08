########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "*.h")

if( CCOS_GUI )
	add_to_kernel_libraries ( X11 )
endif()

add_to_kernel_sources    ( ${SOURCE_FILES} )
add_to_kernel_definitions( ${DEFINITIONS} )
add_to_kernel_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} )
add_to_kernel_libraries  ( pthread
                           crypt )
