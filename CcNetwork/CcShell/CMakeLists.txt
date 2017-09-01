if(NOT DEFINED CCOS_CCNETWORK_CCSHELL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCNETWORK_CCSHELL_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcShell )
  
  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "src/*.c"
        "src/*.cpp"
        "config/*.xml"
        "*.h")
  
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT})
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
  endif()
   
  add_library( ${CURRENT_PROJECT} ${CCOS_LINK_TYPE} ${SOURCE_FILES} )
  
  # To keep Header an Source together, display them in same Directory
  # It will only work with IDE's wich are supporting this features
  SOURCE_GROUP( ""        FILES ${SOURCE_FILES})
  SOURCE_GROUP( "config"  FILES "config/default.xml")
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcNetwork")
  
  target_link_libraries ( ${CURRENT_PROJECT} PUBLIC
                                CcKernel
                                CcMedia )
      
  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSSetInstall( ${CURRENT_PROJECT} )
  CcOSSetInstallConfig ( ${CURRENT_PROJECT} )
endif()