################################################################################
# Default Windows Config
################################################################################
message("- Default Configuration for Windows is selected")

################################################################################
# Setup default directories
################################################################################
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set( CMAKE_INSTALL_PREFIX       "C:/Program Files/CcOS/" )
endif()

set(WINDOWS TRUE)
