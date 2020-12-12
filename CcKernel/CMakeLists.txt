if(NOT DEFINED CCOS_CCKERNEL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCKERNEL_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  ########
  # Add Source Files
  ########
  set (KERNEL_SOURCE_FILES
    CcApp.cpp
    CcApp.h
    CcAppDirectories.cpp
    CcAppDirectories.h
    CcAppKnownList.cpp
    CcAppKnownList.h
    CcAppList.cpp
    CcAppList.h
    CcAppListItem.cpp
    CcAppListItem.h
    CcConsole.cpp
    CcConsole.h
    CcConversionTables.cpp
    CcConversionTables.h
    CcDebug.cpp
    CcDebug.h
    CcDevice.cpp
    CcDevice.h
    CcDeviceList.cpp
    CcDeviceList.h
    CcDirectory.cpp
    CcDirectory.h
    CcEventAction.cpp
    CcEventAction.h
    CcEventActionList.h
    CcEventActionLoop.cpp
    CcEventActionLoop.h
    CcEvent.cpp
    CcEvent.h
    CcEventHandleMap.h
    CcEventHandler.cpp
    CcEventHandler.h
    CcFile.cpp
    CcFile.h
    CcFileInfo.cpp
    CcFileInfo.h
    CcFileInfoList.cpp
    CcFileInfoList.h
    CcFileSystem.cpp
    CcFileSystem.h
    CcFileSystemListItem.cpp
    CcFileSystemListItem.h
    CcInputEvent.cpp
    CcInputEvent.h
    CcIoAsync.cpp
    CcIoAsync.h
    CcKernel.cpp
    CcKernel.h
    CcLog.cpp
    CcLog.h
    CcMacros.h
    CcMemoryManager.cpp
    CcMemoryManager.h
    CcMemoryMonitor.cpp
    CcMemoryMonitor.h
    CcMutex.cpp
    CcMutex.h
    CcObject.cpp
    CcObject.h
    CcOSVersion.h
    CcProcess.cpp
    CcProcess.h
    CcSharedMemory.cpp
    CcSharedMemory.h
    CcStdErr.cpp
    CcStdErr.h
    CcStdFile.cpp
    CcStdFile.h
    CcStdIn.cpp
    CcStdIn.h
    CcStdOut.cpp
    CcStdOut.h
    CcSystem.h
    CcThreadContext.h
    CcThreadManager.cpp
    CcThreadManager.h
    CcTimer.cpp
    CcTimer.h
    EEventType.h
    EInputEventTypes.h
    IDevice.cpp
    IDevice.h
    IDriver.h
    IFile.h
    IFileSystem.h
    IKernel.h
    IModuleBase.cpp
    IModuleBase.h
    IProgressReceiver.h
    ISharedMemory.h
    IThread.cpp
    IThread.h
    IWaitable.cpp
    IWaitable.h
    IWorker.cpp
    IWorker.h
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

  macro (push_up_kernel_vars)
    add_to_kernel_sources     ( ${KERNEL_SOURCES} )
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
  add_subdirectory(Network/)
  add_subdirectory(Devices/)
  add_subdirectory(Platform/)
  add_subdirectory(Events/)
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
  
  CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${KERNEL_SOURCE_FILES} ${KERNEL_TYPE_FILES} )

  if(${KERNEL_DEPENDENCIES})
    add_dependencies(${CURRENT_PROJECT} ${KERNEL_DEPENDENCIES})
  endif()

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS")

  target_link_libraries ( ${CURRENT_PROJECT} ${KERNEL_LIBRARIES} )

  foreach(scriptfile ${KERNEL_SCRIPTS})
    if(NOT ${scriptfile} STREQUAL "")
      target_link_libraries("${CURRENT_PROJECT}" "-T ${scriptfile}")
    endif()
  endforeach()

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} ${KERNEL_INCLUDES})
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${KERNEL_SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif(NOT DEFINED CCOS_CCKERNEL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
