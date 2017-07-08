# Set Compiler Settings
MESSAGE("Raspberry-Config activated")

set(CCOS_THIRDPARTY         TRUE)
set(CCOS_THIRDPARTY_LIBJPG  TRUE)
set(CCOS_THIRDPARTY_SQLITE  TRUE)
set(CCOS_PLATFORM           TRUE)
set(CCOS_PLATFORM_RASPBIAN  TRUE)
set(CCOS_DRIVER             TRUE)
set(CCOS_DRIVER_CAMERA      TRUE)
set(CCOS_DRIVER_V4L         TRUE)

if(DEFINED CMAKE_TOOLCHAIN_FILE)
    add_subdirectory(Toolchain/Raspberry/userland)
endif()
