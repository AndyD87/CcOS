################################################################################
# Check if Modules are required for Applications
# Modules can add additional Applications or Modifcations
# at runtime
################################################################################
if( CCOS_PLATFORM )
  add_subdirectory( Platform/ )
endif()
