if(NOT DEFINED CCOS_CCAPPS_CCHASH_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCAPPS_CCHASH_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  
  set(CURRENT_PROJECT CcHash)
  
  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "src/*.c"
        "src/*.cpp"
        "*.h")
  
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT})
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
  endif()
  
  add_executable( ${CURRENT_PROJECT} ${SOURCE_FILES} )
  
  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                CcKernel )
  
  # To keep Header an Source together, display them in same Directory
  # It will only work with IDE's wich are supporting this features                              
  source_group( ""        FILES ${SOURCE_FILES})
                                
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcApps")
  
  #################
  # Post configuration steps
  #################
  CcOSSetInstall( ${CURRENT_PROJECT} )

endif()
