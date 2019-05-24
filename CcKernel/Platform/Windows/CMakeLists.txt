cmake_minimum_required (VERSION 2.6)
########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.c"
      "*.cpp"
      "USB/*.cpp"
      "*.h"
      "USB/*.h")

if(CCOS_DRIVER_NETWORK_WINDOWS)
  set(DEFINITIONS -DWINDOWS_NETWORK_STACK)
endif(CCOS_DRIVER_NETWORK_WINDOWS)

########
# Add to kernel
########

add_to_kernel_sources    ( ${SOURCE_FILES} )
add_to_kernel_definitions( ${DEFINITIONS} )
add_to_kernel_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} )
add_to_kernel_libraries  ( gdiplus
                           setupapi
                           hid
                           Ws2_32
                           Netapi32 )
