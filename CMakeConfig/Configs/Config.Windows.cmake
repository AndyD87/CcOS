################################################################################
# Default Windows Config
################################################################################
MESSAGE("Default Configuration for Windows is choosen\n")

################################################################################
# Setup default directories
################################################################################
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set( CMAKE_INSTALL_PREFIX       "C:/Program Files/CcOS/" )
endif()

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