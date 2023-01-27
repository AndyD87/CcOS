set(CURRENT_PROJECT           CcUsbDriver )
set(CURRENT_PROJECT_IDE_PATH  "CcApps"    )

CcGenerateVersion(CcUsbDriver 0 0 0 1)
string(TIMESTAMP CcUsbDriver_TIMESTAMP "%m/%d/%Y")

if(CCKERNEL_MODULE_AVAILABLE)
        
  configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/CcUsbDriver.inf.in
        ${CMAKE_CURRENT_BINARY_DIR}/CcUsbDriver.inf
        @ONLY
  )
  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcUsbDriver.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcUsbDriver.h
        ${CMAKE_CURRENT_LIST_DIR}/CcUsbDevice.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcUsbDevice.h
        ${CMAKE_CURRENT_LIST_DIR}/CcUsbDriver.inf.in
        ${CMAKE_CURRENT_BINARY_DIR}/CcUsbDriver.inf
  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddDriver( ${CURRENT_PROJECT} ${SOURCE_FILES})

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                CcKernelModule
  )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  set_target_properties(${CURRENT_PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

  #################
  # Post configuration steps
  #################
  CcTargetInstall(${CURRENT_PROJECT})
  CcOSExeSettings(${CURRENT_PROJECT})
  CcSetFiltersByFolders(${SOURCE_FILES})

endif()
