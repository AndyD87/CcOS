########
# Add Source Files
########
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/CcLinux.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxBoardSupport.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxBoardSupport.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxFile.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxFile.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxFilesystem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxFilesystem.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxGpioPin.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxGpioPin.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxGpioPort.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxGpioPort.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxI2C.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxI2C.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxLed.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxLed.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxModule.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxModule.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxNetworkStack.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxNetworkStack.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxNetworkDevice.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxNetworkDevice.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxPipe.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxPipe.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxProcessThread.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxProcessThread.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxServiceSystem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxSharedMemory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxSharedMemory.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxSocketTcp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxSocketTcp.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxSocketUdp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxSocketUdp.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxSystem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxTimer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxTimer.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxTouch.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxTouch.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxUser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxUser.h
        ${CMAKE_CURRENT_LIST_DIR}/ILinuxSocket.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ILinuxSocket.h
)

add_to_kernel_sources    ( ${SOURCE_FILES} )
add_to_kernel_definitions( ${DEFINITIONS} )
add_to_kernel_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} )
add_to_kernel_libraries  ( pthread rt dl
                           crypt )
