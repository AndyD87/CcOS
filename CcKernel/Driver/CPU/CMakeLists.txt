################################################################################
# Check if Modules are required for Applications by reading config
################################################################################
if( CCOS_DRIVER_CPU_ST)
  add_subdirectory(ST)
elseif(CCOS_DRIVER_CPU_ESPRESSIF)
  add_subdirectory(espressif)
endif ()

file (GLOB SOURCE_FILES
      "Common/*.cpp"
      "Common/*.c" )

########
# Add Source Files
########
file (GLOB HEADER_FILES
      "*h" )

add_to_driver_sources(${HEADER_FILES} ${SOURCE_FILES})

push_up_driver_vars()
