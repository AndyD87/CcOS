###################
# Default Windows Config
###################
MESSAGE("Default Configuration for Windows is choosen\n")

###################
# Setup default directories
###################

set( CMAKE_INSTALL_PREFIX       "C:/Program Files/CcOS/" )

###################
# Enable or Disable Modules
###################

# Enable Camera Drivers for Windows
set( USE_DRIVER                 y)
set( USE_DRIVER_CAMERA          y)
# Windows Desktop Screen as Camera Device
set( USE_WINDOWS_DESKTOP_SCREEN y) 


set( USE_THIRDPARTY         y)
set( USE_THIRDPARTY_LIBJPG  y)
set( USE_THIRDPARTY_SQLITE  y)
set( USE_THIRDPARTY_OPENSSL y)