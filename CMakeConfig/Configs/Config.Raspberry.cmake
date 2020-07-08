# Set Compiler Settings
message("- Raspberry-Config activated")

set(CCOS_THIRDPARTY         TRUE)
set(CCOS_THIRDPARTY_LIBJPG  TRUE)
set(CCOS_THIRDPARTY_SQLITE  TRUE)
set(CCOS_PLATFORM           TRUE)
set(CCOS_PLATFORM_RASPBIAN  TRUE)
set(CCOS_DRIVER             TRUE)
set(CCOS_DRIVER_CAMERA      TRUE)
set(CCOS_DRIVER_V4L         TRUE)

if(DEFINED CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_CURRENT_LIST_DIR}/../Toolchain/Raspberry/userland)
endif()
