if(NOT DBus_FOUND)
  find_package(DBus QUIET)
endif()

# libdbus-c++-1 libdbus-1
if(DBus_FOUND)
  message("- DBus found")
endif()

if(DBus_FOUND)
  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxDbus.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxDbus.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxDbusArguments.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxDbusArguments.h
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxDbusModule.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcLinuxDbusModule.h
        ${CMAKE_CURRENT_LIST_DIR}/ILinuxDbus.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ILinuxDbus.h
  )

  # Search for NetworkManager
  find_package(NetworkManager QUIET)
  if(NetworkManager_FOUND)
    list(APPEND SOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/Objects/CNetworkManager.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/Objects/CNetworkManager.h"
      "${CMAKE_CURRENT_LIST_DIR}/Objects/CWlanDevice.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/Objects/CWlanDevice.h")
      add_definitions(-DNetworkManager_FOUND)
  endif(NetworkManager_FOUND)

  # If required for kernel load it now
  if(CCOS_CCKERNEL_DRIVER_NETWORK_DBUS_ACTIVTE)
    add_to_driver_headers(Network/DBus/CcLinuxDbusModule.h)
    add_to_driver_names  (4 CcLinuxDbusModule)
    add_to_driver_libraries(${DBus_LIBRARIES})
    add_to_driver_sources(${SOURCE_FILES})
    add_to_kernel_definitions( -DCCOS_KERNEL_DBUS )
  else()
    # Generate a loadable module
    set ( CURRENT_PROJECT           "dbus.ccm" )
    set ( CURRENT_PROJECT_IDE_PATH  "Driver/Network/DBus" )
    if(WINDOWS)
      CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
    endif()
    CcAddLibrary( ${CURRENT_PROJECT} SHARED ${SOURCE_FILES})

    set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

    target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                    CcModule ${DBus_LIBRARIES})

    CcTargetIncludes( ${CURRENT_PROJECT} ${DBus_INCLUDE_DIRS} )
    CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})

    CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)
  endif()
endif()

