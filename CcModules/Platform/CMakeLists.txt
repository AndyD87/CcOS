###################
# Check if Modules are required for Applications by reading config
###################
if( CCOS_PLATFORM_RASPBIAN )
  add_subdirectory(Raspbian)
endif()
