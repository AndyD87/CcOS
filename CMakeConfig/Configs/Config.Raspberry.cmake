# Set Compiler Settings
MESSAGE("Raspberry-Config activated")

set(USE_THIRDPARTY         TRUE)
set(USE_THIRDPARTY_LIBJPG  TRUE)
set(USE_THIRDPARTY_SQLITE  TRUE)
set(USE_PLATFORM           TRUE)
set(USE_PLATFORM_RASPBIAN  TRUE)
set(USE_DRIVER             TRUE)
set(USE_DRIVER_CAMERA      TRUE)
set(USE_DRIVER_V4L         TRUE)

if(DEFINED CMAKE_TOOLCHAIN_FILE)
    add_subdirectory(Toolchain/Raspberry/userland)
endif()
