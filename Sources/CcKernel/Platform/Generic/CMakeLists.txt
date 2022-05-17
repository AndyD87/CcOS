########
# Add Source Files
########
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/CcGenericDirectory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcGenericDirectory.h
        ${CMAKE_CURRENT_LIST_DIR}/CcGenericFile.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcGenericFile.h
        ${CMAKE_CURRENT_LIST_DIR}/CcGenericFilesystem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcGenericFilesystem.h
        ${CMAKE_CURRENT_LIST_DIR}/CcGenericThreadHelper.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcGenericThreadHelper.h
        ${CMAKE_CURRENT_LIST_DIR}/CcGenericThreadHelper_prvt.h
        ${CMAKE_CURRENT_LIST_DIR}/CcGenericTimer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcGenericTimer.h
        ${CMAKE_CURRENT_LIST_DIR}/CcServiceSystem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcSystem.cpp
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
  set( LWIP_SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/Network/LwipStack/ILwipSocket.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Network/LwipStack/ILwipSocket.h
        ${CMAKE_CURRENT_LIST_DIR}/Network/LwipStack/LwipNetworkStack.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Network/LwipStack/LwipNetworkStack.h
        ${CMAKE_CURRENT_LIST_DIR}/Network/LwipStack/LwipSocketTcp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Network/LwipStack/LwipSocketTcp.h
        ${CMAKE_CURRENT_LIST_DIR}/Network/LwipStack/LwipSocketUdp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Network/LwipStack/LwipSocketUdp.h
  )
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
