###
# Setup Toolchain if required
###
message("- Load toolchain: esp-open-sdk")
set(DOWNLOAD_ONLY TRUE)
include(${CMAKE_CURRENT_LIST_DIR}/../../../Toolchains/esp-open-sdk/Toolchain.cmake)
set(DOWNLOAD_ONLY FALSE)
if(CMAKE_TOOLCHAIN_FILE)
  include(${CMAKE_TOOLCHAIN_FILE})
endif()
include(${CMAKE_CURRENT_LIST_DIR}/../../../Toolchains/esp-open-sdk/Toolchain.cmake)

# Setup CPU-Config
include(${CMAKE_CURRENT_LIST_DIR}/../../../CPU/espressif/ESP8266/Config.cmake)
