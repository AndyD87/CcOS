if(NOT DEFINED CCOS_CCAPPS_CCREMOTEDEVICESERVER_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCAPPS_CCREMOTEDEVICESERVER_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set(CURRENT_PROJECT CcRemoteDeviceServer)

  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.c"
        "*.cpp"
        "*.h")

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()

  add_executable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                CcKernel
                                CcRemoteDevice )
  
  # To keep Header an Source together, display them in same Directory
  # It will only work with IDE's wich are supporting this features                              
  source_group( ""        FILES ${SOURCE_FILES})
                              
  set_target_properties(${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/CcApps")

  #################
  # Post configuration steps
  #################
  CcOSSetInstall( ${CURRENT_PROJECT} )

endif()