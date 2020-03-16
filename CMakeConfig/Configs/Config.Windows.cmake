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

################################################################################
# Enable or Disable Modules
################################################################################

set( CCOS_THIRDPARTY         y)
set( CCOS_THIRDPARTY_LIBJPG  y)
set( CCOS_THIRDPARTY_SQLITE  y)
set( CCOS_THIRDPARTY_OPENSSL y)
