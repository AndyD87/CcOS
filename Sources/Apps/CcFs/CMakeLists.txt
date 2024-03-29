set(CURRENT_PROJECT           CcFs )
set(CURRENT_PROJECT_IDE_PATH  "CcApps"    )

CcGenerateVersion(CcFs 0 0 0 1)
string(TIMESTAMP CcFs_TIMESTAMP "%m/%d/%Y")

if(CCKERNEL_MODULE_AVAILABLE)
  configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/CcFs.inf.in
        ${CMAKE_CURRENT_BINARY_DIR}/CcFs.inf
        @ONLY
  )
  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcFsDriver.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcFsDriver.h
        ${CMAKE_CURRENT_LIST_DIR}/CcFsDevice.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcFsDevice.h
        ${CMAKE_CURRENT_LIST_DIR}/CcFs.inf.in
        ${CMAKE_CURRENT_BINARY_DIR}/CcFs.inf
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
  CcOSExeSettings(${CURRENT_PROJECT})
  CcSetFiltersByFolders(${SOURCE_FILES})

endif()
