set(CURRENT_PROJECT           CcShellApp          )
set(CURRENT_PROJECT_IDE_PATH  CcApps              )

if(NOT DEFINED CCOS_APPS_CCSHELLAPP_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(CCOS_NO_APPS)
  message("- Skip-App: ${CURRENT_PROJECT}")
elseif(${CCOS_APPS_CCSHELLAPP_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcShellAppWorker.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcShellAppWorker.h
        ${CMAKE_CURRENT_LIST_DIR}/CcShellApp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcShellApp.h
  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                CcKernel
                                CcShell
                                CcSsl
  )

  set_target_properties(${CURRENT_PROJECT} PROPERTIES FOLDER      "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  set_target_properties(${CURRENT_PROJECT} PROPERTIES OUTPUT_NAME CcShell)

  #################
  # Post configuration steps
  #################
  CcTargetInstall(${CURRENT_PROJECT})
  CcOSExeSettings(${CURRENT_PROJECT})
  CcSetFiltersByFolders(${SOURCE_FILES})
endif()
