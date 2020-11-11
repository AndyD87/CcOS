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

find_package(WDK)

if(WDK_FOUND)
  message("-- WDK available")
  set(CCKERNEL_MODULE_AVAILABLE TRUE)
  file (GLOB CCKERNEL_MODULE_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/Windows/*.cpp"
  )
endmacro()
