cmake_minimum_required (VERSION 2.6)
########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "USB/src/*.cpp"
      "*.h"
      "USB/*.h")

set(DEFINITIONS -DWINDOWSSYSTEM)

########
# Add to kernel
########

add_to_kernel_sources    ( ${SOURCE_FILES} )
add_to_kernel_definitions( ${DEFINITIONS} )
add_to_kernel_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} )
add_to_kernel_libraries  ( jpeg
                           gdiplus
                           setupapi
                           hid
                           Ws2_32
                           Netapi32 )
