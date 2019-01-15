if(NOT DEFINED CCOS_CCMODULES_PLATFORM_RASPBIAN_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCMODULES_PLATFORM_RASPBIAN_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.c" )
  
  
  ########
  # Add Source Files
  ########
  file (GLOB HEADER_FILES
        "*h" )
  
  
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
             
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()
             
  add_library(modRaspbian ${CC_LINK_TYPE} ${SOURCE_FILES} ${HEADER_FILES} )
  
  link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../../lib )
  
  set_property( TARGET modRaspbian PROPERTY FOLDER "CcOS")
  
  target_link_libraries ( modRaspbian LINK_PRIVATE
                                CcKernel )
  
  target_include_directories( modRaspbian PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif()