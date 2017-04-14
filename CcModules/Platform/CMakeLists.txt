###################
# Check if Modules are required for Applications by reading config
###################
if( USE_PLATFORM_RASPBIAN )
  add_subdirectory(Raspbian)
endif()
