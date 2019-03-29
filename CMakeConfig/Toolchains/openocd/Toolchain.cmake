##
# This file will download openocd for debugging on Windows.
# For linux, you can use openocd from distribution like:
#   apt-get install openocd
##
if( WIN32 )
  set( WORKING_DIR "${CC_CACHE_DIR}/Toolchains" )
  set( OPENOCD_DIR "${WORKING_DIR}/openocd" )
  CcDownloadAndExtract( "openocd" 
                        "${OPENOCD_DIR}"
                        "http://coolcow.de/projects/ThirdParty/openocd/binaries/0.10.0.10/gnu-mcu-eclipse-openocd.win32.zip")
endif()
