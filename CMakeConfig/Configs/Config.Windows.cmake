################################################################################
# Default Windows Config
################################################################################
MESSAGE("Default Configuration for Windows is choosen\n")

################################################################################
# Setup default directories
################################################################################

set( CMAKE_INSTALL_PREFIX       "C:/Program Files/CcOS/" )

################################################################################
# Enable or Disable Modules
################################################################################

# Enable Camera Drivers for Windows
set( CCOS_DRIVER                 y)
set( CCOS_DRIVER_CAMERA          y)
# Windows Desktop Screen as Camera Device
set( CCOS_WINDOWS_DESKTOP_SCREEN y) 


set( CCOS_THIRDPARTY         y)
set( CCOS_THIRDPARTY_LIBJPG  y)
set( CCOS_THIRDPARTY_SQLITE  y)
set( CCOS_THIRDPARTY_OPENSSL y)