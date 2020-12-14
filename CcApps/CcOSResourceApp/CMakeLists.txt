set(CURRENT_PROJECT CcOSResource)

if(NOT DEFINED CCOS_CCAPPS_CCOSRESOURCEAPP_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(CCOS_NO_APPS)
  message("- Skip-App: ${CURRENT_PROJECT}")
elseif(${CCOS_CCAPPS_CCOSRESOURCEAPP_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  ########
  # Add Source Files
  ########
  set( SOURCE_FILES
          main.cpp
  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                CcKernel
                                CcOSBuildConfig )

  # To keep Header an Source together, display them in same Directory
  # It will only work with IDE's wich are supporting this features
  source_group( ""        FILES ${SOURCE_FILES})

  set_target_properties(${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/CcApps")

  #################
  # Post configuration steps
  #################
  CcOSSetInstall( ${CURRENT_PROJECT} )
  CcOSExeSettings(${CURRENT_PROJECT})
endif()
