###
# Setup Toolchain if required
###
if(NOT CMAKE_TOOLCHAIN_FILE)
    message("- Load toolchain: esp-open-sdk")
    include(${CMAKE_CURRENT_LIST_DIR}/../../../Toolchains/esp-open-sdk/Toolchain.cmake)
endif()


# Setup CPU-Config
include(${CMAKE_CURRENT_LIST_DIR}/../../../CPU/espressif/ESP8266/Config.cmake)

