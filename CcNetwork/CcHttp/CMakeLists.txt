if(NOT DEFINED CCOS_CCNETWORK_CCHTTP_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCNETWORK_CCHTTP_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcHttp )
  
  ########
  # Add Source Files
  ########
  file (GLOB HTTP_SOURCE_FILES
        "src/*.c"
        "src/*.cpp"
        "config/*.xml"
        "*.h")
  
  macro (add_to_http_sources)
      foreach (_src ${ARGN})
          CcListAppendOnce(HTTP_SOURCE_FILES "${_src}")
      endforeach()
      set (HTTP_SOURCE_FILES ${HTTP_SOURCE_FILES} PARENT_SCOPE)
  endmacro()
  
  ########
  # Add Subdirecotry Files
  ########
  add_subdirectory(HttpProvider/)
  
  set( DEFINITIONS "" )
  set( LIBRARIES  CcKernel )

  if( CCOS_THIRDPARTY_OPENSSL_ACTIVE GREATER ${CCOS_BUILDLEVEL} )
    list( APPEND LIBRARIES CcSsl)
    list( APPEND DEFINITIONS -DCCSSL_ENABLED)
  endif()
  
  add_definitions( ${DEFINITIONS} )
  
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()
   
  add_library( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${HTTP_SOURCE_FILES} )
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcNetwork")
  
  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE ${LIBRARIES} )
  
  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
endif()
