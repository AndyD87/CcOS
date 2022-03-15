set(CURRENT_PROJECT CcHash)

if(NOT DEFINED CCOS_APPS_IHash_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(CCOS_NO_APPS)
  message("- Skip-App: ${CURRENT_PROJECT}")
elseif(${CCOS_APPS_IHash_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                CcKernel )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcApps")

  #################
  # Post configuration steps
  #################
  CcOSSetInstall( ${CURRENT_PROJECT} )
  CcSetFiltersByFolders(${SOURCE_FILES})

endif()
