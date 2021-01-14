########
# Add Source Files
########
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/CcWindows.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsFile.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsFile.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsFilesystem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsFilesystem.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsGlobals.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsGlobals.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsModule.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsModule.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsNetworkStack.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsNetworkStack.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsPipe.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsPipe.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsProcessThread.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsProcessThread.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsRegistryFile.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsRegistryFile.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsRegistryFilesystem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsRegistryFilesystem.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsService.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsService.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsServiceControl.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsServiceControl.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsSharedMemory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsSharedMemory.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsSocketTcp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsSocketTcp.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsSocketUdp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsSocketUdp.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsSystem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsTimer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsTimer.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsTouch.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsTouch.h
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsUser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcWindowsUser.h
        ${CMAKE_CURRENT_LIST_DIR}/IWindowsSocket.cpp
        ${CMAKE_CURRENT_LIST_DIR}/IWindowsSocket.h
        ${CMAKE_CURRENT_LIST_DIR}/USB/WinUSBHid.cpp
        ${CMAKE_CURRENT_LIST_DIR}/USB/WinUSBHid.h
)

if(CCOS_GENERIC_NETWORK_WINDOWS)
  set(DEFINITIONS -DWINDOWS_NETWORK_STACK)
endif(CCOS_GENERIC_NETWORK_WINDOWS)

########
# Add to kernel
########

add_to_kernel_sources    ( ${SOURCE_FILES} )
add_to_kernel_definitions( ${DEFINITIONS} )
add_to_kernel_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} )
if(NOT DEFINED GCC)
  add_to_kernel_libraries  ( Mincore )
else()
  add_to_kernel_libraries  ( version userenv )
endif()
add_to_kernel_libraries  ( gdiplus
                           setupapi
                           shell32
                           shfolder
                           hid
                           ws2_32
                           netapi32
                           version
)
