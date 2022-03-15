if(NOT DEFINED CCOS_CCKERNELMODULE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif( ${CCOS_CCKERNELMODULE_ACTIVE} GREATER ${CCOS_BUILDLEVEL} AND
        CCKERNEL_MODULE_AVAILABLE)

  set ( CURRENT_PROJECT           CcKernelModule )
  set ( CURRENT_PROJECT_IDE_PATH  ""    )

  #########
  ## Add Source Files
  #########
  file (GLOB KERNEL_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/../CcKernel/Types/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../CcKernel/Types/Hash/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../CcKernel/Network/CcCommonIps.cpp"
  )

  file (GLOB KERNEL_HEADER_FILES
        "${CMAKE_CURRENT_LIST_DIR}/../CcKernel/Types/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/../CcKernel/Types/Hash/*.h"
  )

  file (GLOB SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/CcKernelModule.cpp"
  )

  file (GLOB HEADER_FILES
        "${CMAKE_CURRENT_LIST_DIR}/CcMalloc.h"
        "${CMAKE_CURRENT_LIST_DIR}/CcKernelModule.h"
        "${CMAKE_CURRENT_LIST_DIR}/CcConnection.h"
        "${CMAKE_CURRENT_LIST_DIR}/CcRequest.h"
        "${CMAKE_CURRENT_LIST_DIR}/IDriver.h"
        "${CMAKE_CURRENT_LIST_DIR}/IDevice.h"
        "${CMAKE_CURRENT_LIST_DIR}/IDeviceInterface.h"
        "${CMAKE_CURRENT_LIST_DIR}/IFsDevice.h"
        "${CMAKE_CURRENT_LIST_DIR}/IFsDriver.h"
  )

  set(CCKERNELMODULE_OBJECT "/" CACHE INTERNAL "")

  set(CCKERNELMODULE_OBJECT_PATH "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"                        CACHE INTERNAL "")
  set(CCKERNELMODULE_OBJECT_NAME "lib${CURRENT_PROJECT}.a"                                  CACHE INTERNAL "")
  set(CCKERNELMODULE_OBJECT "${CCKERNELMODULE_OBJECT_PATH}/${CCKERNELMODULE_OBJECT_NAME}"   CACHE INTERNAL "")

  if(WINDOWS)
    set( WINDOWS_BUILD_FILES
        "${CMAKE_CURRENT_LIST_DIR}/Windows/CcConnection.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/CcKernelModuleContext.h"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/CcMalloc.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/CcRequest.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/CcRequestContext.h"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/CppKernelRuntime.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/IDevice.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/IDeviceContext.h"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/IDeviceInterfaceContext.h"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/IDeviceInterface.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/IDriver.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/IDriverContext.h"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/IFsDevice.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/IFsDriver.cpp"
    )
    include_directories(
      "${CMAKE_CURRENT_LIST_DIR}/Windows"
    )
    list(APPEND KERNEL_SOURCE_FILES ${WINDOWS_BUILD_FILES})
    list(APPEND KERNEL_MODULE_DEFINITIONS -DWINDOWSKERNEL)
  elseif(LINUX AND NOT CCOS_BOARD_TYPE)
    set( LINUX_BUILD_FILES
        "${CMAKE_CURRENT_LIST_DIR}/Linux/CcConnection.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/CcKernelModuleContext.h"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/CcMalloc.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/CcRequest.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/CcRequestContext.h"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/IDevice.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/IDeviceContext.h"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/IDeviceInterfaceContext.h"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/IDeviceInterface.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/IDriver.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/IDriverContext.h"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/IFsDevice.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/IFsDriver.cpp"
    )
    include_directories(
      "${CMAKE_CURRENT_LIST_DIR}/Linux"
    )
    list(APPEND KERNEL_SOURCE_FILES ${LINUX_BUILD_FILES})
    list(APPEND KERNEL_MODULE_DEFINITIONS -DLINUXKERNEL)
  endif()

  include_directories( ${CMAKE_CURRENT_LIST_DIR}              )
  include_directories( ${CCKERNEL_DIR}                        )
  include_directories( ${CCKERNEL_TYPES_DIR}                  )
  include_directories( ${CCKERNEL_MODULE_INCLUDE_DIRS}        )

  include_directories(
    "${CMAKE_CURRENT_LIST_DIR}/../CcKernel/Types"
  )

  CcAddDriverLibrary( ${CURRENT_PROJECT} ${KERNEL_SOURCE_FILES} ${SOURCE_FILES})

  set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS")
  set_target_properties( ${CURRENT_PROJECT} PROPERTIES PUBLIC_HEADER "${KERNEL_HEADER_FILES};${HEADER_FILES}")

  set_target_properties(${CURRENT_PROJECT} PROPERTIES LINKER_LANGUAGE CXX)
  target_compile_definitions(${CURRENT_PROJECT} PUBLIC ${KERNEL_MODULE_DEFINITIONS})

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} "${CMAKE_CURRENT_LIST_DIR}/../CcKernel/Types" )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${KERNEL_SOURCE_FILES} ${HEADER_FILES})
  CcSetFiltersByFolders(${SOURCE_FILES} ${KERNEL_SOURCE_FILES})
endif()
