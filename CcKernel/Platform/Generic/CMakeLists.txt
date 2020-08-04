cmake_minimum_required (VERSION 2.6)
########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.c"
      "*.cpp"
      "*.h"
)

if(CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT)
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Features/CcOS_malloc.h")
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Features/CcOS_malloc.cpp")
endif()

if(CCOS_CCKERNEL_GENERIC_PRINTF)
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Features/CcOS_printf.cpp")
endif()

if(CCOS_CCKERNEL_GENERIC_NO_SYSTEM_THREAD)
  set(DEFINITIONS "${DEFINITIONS} -DCCOS_NO_SYSTEM_THREAD")
endif()

if(CCOS_CCKERNEL_GENERIC_NETWORK_STACK)
  set(DEFINITIONS "${DEFINITIONS} -DCCOS_CCKERNEL_GENERIC_NETWORK_STACK")
elseif(CCOS_CCKERNEL_GENERIC_LWIP_STACK)
  set(DEFINITIONS "${DEFINITIONS} -DCCOS_CCKERNEL_GENERIC_LWIP_STACK")
  file (GLOB LWIP_SOURCE_FILES
        "Network/LwipStack/*.c"
        "Network/LwipStack/*.cpp"
        "Network/LwipStack/*.h")
  list(APPEND SOURCE_FILES "${LWIP_SOURCE_FILES}")
endif(CCOS_CCKERNEL_GENERIC_NETWORK_STACK)

if(CCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER)
  set(DEFINITIONS "${DEFINITIONS} -DCCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER")
endif(CCOS_CCKERNEL_GENERIC_NO_SYSTEM_TIMER)

set(EXT_INC_LIST "")
foreach(file_path ${EXT_HEADER_FILES})
  get_filename_component(dir_path ${file_path} PATH)
  list(APPEND EXT_INC_LIST ${dir_path})
endforeach()
list(REMOVE_DUPLICATES EXT_INC_LIST)

########
# Add subdirectories
########

########
# Add to kernel
########

add_to_kernel_definitions( ${DEFINITIONS} )
add_to_kernel_sources    (  ${EXT_HEADER_FILES}
                            ${SOURCE_FILES})
add_to_kernel_includes   ( ${CMAKE_CURRENT_SOURCE_DIR})
