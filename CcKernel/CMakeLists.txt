if(NOT DEFINED CCOS_CCKERNEL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCKERNEL_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  ########
  # Define public header
  ########
  set (KERNEL_HEADER_FILES
    CcAppDirectories.h
    CcApp.h
    CcAppKnownList.h
    CcAppList.h
    CcAppListItem.h
    CcConsole.h
    CcConversionTables.h
    CcDevice.h
    CcDebug.h
    CcDeviceList.h
    CcDirectory.h
    CcEventAction.h
    CcEventActionList.h
    CcEventActionLoop.h
    CcEvent.h
    CcEventHandleMap.h
    CcEventHandler.h
    CcFile.h
    CcFileInfo.h
    CcFileInfoList.h
    CcFileSystem.h
    CcFileSystemListItem.h
    CcInputEvent.h
    CcIoAsync.h
    CcKernel.h
    CcLog.h
    CcMacros.h
    CcMemoryMonitor.h
    CcMemoryManager.h
    CcObject.h
    CcOSVersion.h
    CcProcess.h
    CcService.h
    CcSharedMemory.h
    CcStdErr.h
    CcStdFile.h
    CcStdIn.h
    CcThreadManager.h
    CcStdOut.h
    CcServiceSystem.h
    CcSystem.h
    CcThreadContext.h
    CcTimer.h
    EEventType.h
    EInputEventTypes.h
    IDevice.h
    IDriver.h
    IFile.h
    IFileSystem.h
    IKernel.h
    IModuleBase.h
    IProgressReceiver.h
    ISharedMemory.h
    IThread.h
    IWaitable.h
    IWorker.h

    Devices/CcBoardSupport.h
    Devices/CcDeviceCamera.h
    Devices/CcDeviceClock.h
    Devices/CcDeviceCpu.h
    Devices/CcDeviceDisplay.h
    Devices/CcDeviceEeprom.h
    Devices/CcDeviceGpioPin.h
    Devices/CcDeviceGpioPort.h
    Devices/CcDeviceGrayscaleMap.h
    Devices/CcDeviceHdd.h
    Devices/CcDeviceI2C.h
    Devices/CcDeviceIoDevice.h
    Devices/CcDeviceLed.h
    Devices/CcDeviceNetwork.h
    Devices/CcDevicePixMap.h
    Devices/CcDeviceSdCard.h
    Devices/CcDeviceSpi.h
    Devices/CcDeviceTimer.h
    Devices/CcDeviceTouch.h
    Devices/CcDeviceUart.h
    Devices/CcDeviceUsb.h
    Devices/CcDeviceUsbHid.h
    Devices/CcDeviceWlan.h
    Devices/CcDeviceWlanAccessPoint.h
    Devices/CcDeviceWlanClient.h
    Devices/CcGpioPortPin.h
    Devices/Devices.h
    Devices/IBoardSupport.h
    Devices/ICamera.h
    Devices/IClock.h
    Devices/ICpu.h
    Devices/IDisplay.h
    Devices/IEeprom.h
    Devices/IGpioPin.h
    Devices/IHdd.h
    Devices/IGrayscaleMap.h
    Devices/II2C.h
    Devices/IIoDevice.h
    Devices/IGpioPort.h
    Devices/ILed.h
    Devices/INetwork.h
    Devices/IPixMap.h
    Devices/ISdCard.h
    Devices/ITimer.h
    Devices/ITouch.h
    Devices/IUart.h
    Devices/IUsb.h
    Devices/IUsbHid.h
    Devices/IWlan.h
    Devices/IWlanAccessPoint.h
    Devices/IWlanClient.h
    Devices/Simulations/CcBoardSupportSimulation.h
    Devices/Simulations/CcClockSimulation.h
    Devices/Simulations/CcEepromSimulation.h
    Devices/Simulations/CcGpioPortSimulation.h
    Devices/Simulations/CcI2CSimulation.h
    Devices/Simulations/CcSpiSimulation.h

    Network/CcCommonIps.h
    Network/CcCommonPorts.h
    Network/CcIpInterface.h
    Network/CcIpInterfaceList.h
    Network/CcMacAddress.h
    Network/CcSocket.h
    Network/CcSocketAddressInfo.h
    Network/INetworkStack.h
    Network/ISocket.h
    Network/NCommonTypes.h
    Network/Protocols/Dhcp/CcDhcpClient.h
    Network/Protocols/Dhcp/CcDhcpPacket.h
    Network/Server/CcTcpEchoServer.h
    Network/Server/CcUdpEchoServer.h
    Network/Stack/CcArpProtocol.h
    Network/Stack/CcIcmpProtocol.h
    Network/Stack/CcIpProtocol.h
    Network/Stack/CcNetworkPacket.h
    Network/Stack/CcNetworkSocketTcp.h
    Network/Stack/CcNetworkSocketUdp.h
    Network/Stack/CcNetworkStack.h
    Network/Stack/CcPtpProtocol.h
    Network/Stack/CcTcpProtocol.h
    Network/Stack/CcUdpProtocol.h
    Network/Stack/INetworkProtocol.h
    Network/Stack/INetworkSocket.h

    Documents/CcIniFile.h

    Events/CcKeyboardEventHandler.h
    Events/CcMouseEventHandler.h
  )

  ########
  # Add Source Files
  ########
  set (KERNEL_SOURCE_FILES
    CcApp.cpp
    CcAppDirectories.cpp
    CcAppKnownList.cpp
    CcAppList.cpp
    CcAppListItem.cpp
    CcConsole.cpp
    CcConversionTables.cpp
    CcDebug.cpp
    CcDevice.cpp
    CcDeviceList.cpp
    CcDirectory.cpp
    CcEventAction.cpp
    CcEventActionLoop.cpp
    CcEvent.cpp
    CcEventHandler.cpp
    CcFile.cpp
    CcFileInfo.cpp
    CcFileInfoList.cpp
    CcFileSystem.cpp
    CcFileSystemListItem.cpp
    CcInputEvent.cpp
    CcIoAsync.cpp
    CcKernel.cpp
    CcLog.cpp
    CcMemoryManager.cpp
    CcMemoryMonitor.cpp
    CcObject.cpp
    CcProcess.cpp
    CcService.cpp
    CcSharedMemory.cpp
    CcStdErr.cpp
    CcStdFile.cpp
    CcStdIn.cpp
    CcStdOut.cpp
    CcThreadManager.cpp
    CcTimer.cpp
    IDevice.cpp
    IDriver.cpp
    IModuleBase.cpp
    IThread.cpp
    IWaitable.cpp
    IWorker.cpp

    Devices/Simulations/CcBoardSupportSimulation.cpp
    Devices/Simulations/CcClockSimulation.cpp
    Devices/Simulations/CcEepromSimulation.cpp
    Devices/Simulations/CcGpioPortSimulation.cpp
    Devices/Simulations/CcI2CSimulation.cpp
    Devices/Simulations/CcSpiSimulation.cpp
    Devices/CcDeviceEeprom.cpp
    Devices/CcDriverObjects.cpp
    Devices/CcGpioPortPin.cpp
    Devices/IBoardSupport.cpp
    Devices/ICamera.cpp
    Devices/IClock.cpp
    Devices/ICpu.cpp
    Devices/IDisplay.cpp
    Devices/IEeprom.cpp
    Devices/IGpioPin.cpp
    Devices/IGpioPort.cpp
    Devices/IGrayscaleMap.cpp
    Devices/IHdd.cpp
    Devices/II2C.cpp
    Devices/IIoDevice.cpp
    Devices/ILed.cpp
    Devices/ISdCard.cpp
    Devices/ISpi.cpp
    Devices/ISpi.h
    Devices/ITimer.cpp
    Devices/ITouch.cpp
    Devices/IUart.cpp
    Devices/IUsb.cpp
    Devices/IUsbHid.cpp
    Devices/IWlan.cpp
    Devices/IUsb.cpp
    Devices/IUsbHid.cpp
    Devices/IWlan.cpp

    Documents/CcIniFile.cpp

    Events/CcKeyboardEventHandler.cpp
    Events/CcMouseEventHandler.cpp

    Network/CcCommonIps.cpp
    Network/CcIpInterface.cpp
    Network/CcIpInterfaceList.cpp
    Network/CcMacAddress.cpp
    Network/CcSocket.cpp
    Network/CcSocketAddressInfo.cpp
    Network/INetworkStack.cpp
    Network/Protocols/Dhcp/CcDhcpClient.cpp
    Network/Protocols/Dhcp/CcDhcpPacket.cpp
    Network/Protocols/Dhcp/CcDhcpPacketData.cpp
    Network/Protocols/Dhcp/CcDhcpPacketData.h
    Network/Server/CcTcpEchoServer.cpp
    Network/Server/CcUdpEchoServer.cpp
    Network/Stack/CcArpProtocol.cpp
    Network/Stack/CcIcmpProtocol.cpp
    Network/Stack/CcIpProtocol.cpp
    Network/Stack/CcNetworkPacket.cpp
    Network/Stack/CcNetworkSocketTcp.cpp
    Network/Stack/CcNetworkSocketUdp.cpp
    Network/Stack/CcNetworkStack.cpp
    Network/Stack/CcPtpProtocol.cpp
    Network/Stack/CcTcpProtocol.cpp
    Network/Stack/CcUdpProtocol.cpp
    Network/Stack/INetworkProtocol.cpp
    Network/Stack/INetworkSocket.cpp
)

  set ( CURRENT_PROJECT CcKernel)

  set ( KERNEL_LIBRARIES )
  set ( KERNEL_DEFINITIONS -DCcKernel_Build)
  set ( KERNEL_DEPENDENCIES )
  set ( KERNEL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Types )
  set ( KERNEL_SCRIPTS )

  if(TOOLCHAIN_KERNEL_FILES)
    CcListAppendOnce(KERNEL_SOURCE_FILES ${TOOLCHAIN_KERNEL_FILES})
  endif()

  ################################################################################
  # Enable Memory Monitor in kernel if required
  ################################################################################
  if(DEFINED MEMORYMONITOR_CHECK_KERNEL)
    CcListAppendOnce(KERNEL_DEFINITIONS -DMEMORYMONITOR_CHECK_KERNEL)
  endif()

  macro (add_to_kernel_sources)
      foreach (_src ${ARGN})
          CcListAppendOnce(KERNEL_SOURCE_FILES "${_src}")
      endforeach()
      set (KERNEL_SOURCE_FILES ${KERNEL_SOURCE_FILES} PARENT_SCOPE)
  endmacro()

  macro (add_to_kernel_definitions)
      foreach (_src ${ARGN})
          CcListAppendOnce(KERNEL_DEFINITIONS "${_src}")
      endforeach()
      set (KERNEL_DEFINITIONS ${KERNEL_DEFINITIONS} PARENT_SCOPE)
  endmacro()

  macro (add_to_kernel_includes)
      foreach (_src ${ARGN})
          CcListAppendOnce(KERNEL_INCLUDES "${_src}")
      endforeach()
      set (KERNEL_INCLUDES ${KERNEL_INCLUDES} PARENT_SCOPE)
  endmacro()

  macro (add_to_kernel_libraries)
      foreach (_src ${ARGN})
          CcListAppendOnce(KERNEL_LIBRARIES "${_src}")
      endforeach()
      set (KERNEL_LIBRARIES ${KERNEL_LIBRARIES} PARENT_SCOPE)
  endmacro()

  macro (add_to_kernel_dependencies)
      foreach (_src ${ARGN})
          CcListAppendOnce(KERNEL_DEPENDENCIES "${_src}")
      endforeach()
      set (KERNEL_DEPENDENCIES ${KERNEL_DEPENDENCIES} PARENT_SCOPE)
  endmacro()

  macro (add_to_kernel_scripts)
      foreach (_src ${ARGN})
          CcListAppendOnce(KERNEL_SCRIPTS "${_src}")
      endforeach()
      set (KERNEL_SCRIPTS ${KERNEL_SCRIPTS} PARENT_SCOPE)
  endmacro()

  macro (add_to_kernel_headers)
      foreach (_src ${ARGN})
          CcListAppendOnce(KERNEL_HEADERS "${_src}")
      endforeach()
      set (KERNEL_HEADERS ${KERNEL_HEADERS} PARENT_SCOPE)
  endmacro()

  macro (push_up_kernel_vars)
    add_to_kernel_sources     ( ${KERNEL_SOURCES} )
    add_to_kernel_headers     ( ${KERNEL_HEADERS} )
    add_to_kernel_definitions ( ${KERNEL_DEFINITIONS} )
    add_to_kernel_includes    ( ${KERNEL_INCLUDES} )
    add_to_kernel_libraries   ( ${KERNEL_LIBRARIES} )
    add_to_kernel_dependencies( ${KERNEL_DEPENDENCIES} )
    add_to_kernel_scripts     ( ${KERNEL_SCRIPTS} )
  endmacro()

  ########
  # Add Subdirecotry Files
  ########
  add_subdirectory(Driver/)
  add_subdirectory(Platform/)

  include(Types/KernelTypeFiles.cmake)

  ########
  # Update Version Info
  ########
  configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.h.in ${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.h @ONLY)

  add_definitions( ${KERNEL_DEFINITIONS} )

  include_directories( ${KERNEL_INCLUDES} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} KERNEL_SOURCE_FILES)
  endif()

  CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${KERNEL_SOURCE_FILES} ${KERNEL_HEADER_FILES} ${KERNEL_TYPE_FILES} )

  if(${KERNEL_DEPENDENCIES})
    add_dependencies(${CURRENT_PROJECT} ${KERNEL_DEPENDENCIES})
  endif()

  set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS")
  set_target_properties( ${CURRENT_PROJECT} PROPERTIES PUBLIC_HEADER "${KERNEL_HEADER_FILES};${KERNEL_TYPE_HEADER};${KERNEL_TYPE_HASH_HEADER}")

  target_link_libraries ( ${CURRENT_PROJECT} ${KERNEL_LIBRARIES} )
  target_include_directories(${CURRENT_PROJECT} PUBLIC Types)

  foreach(scriptfile ${KERNEL_SCRIPTS})
    if(NOT ${scriptfile} STREQUAL "")
      target_link_libraries("${CURRENT_PROJECT}" "-T ${scriptfile}")
    endif()
  endforeach()

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} ${KERNEL_INCLUDES})
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${KERNEL_SOURCE_FILES} ${KERNEL_HEADER_FILES}  ${KERNEL_TYPE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif(NOT DEFINED CCOS_CCKERNEL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
