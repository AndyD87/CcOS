if(NOT DEFINED CCOS_CCUTIL_CCDOCUMENTS_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCUTIL_CCDOCUMENTS_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcDocuments )
  
  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.c"
        "*.cpp"
        "*.h"     
        "CcConfig/*.c"
        "CcConfig/*.cpp"
        "CcConfig/*.h"
        "CcHtml/*.c"
        "CcHtml/*.cpp"
        "CcHtml/*.h"
        "CcJson/*.c"
        "CcJson/*.cpp"
        "CcJson/*.h" 
        "CcXml/*.c"
        "CcXml/*.cpp"
        "CcXml/*.h"          )
  
  set( LIBRARIES 
        CcKernel )
        
  set (DOCUMENTS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
  
  include_directories( ${DOCUMENTS_INCLUDE_DIRS} )
        
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
  endif()
                  
  add_library(${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcUtil")
  
  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE ${LIBRARIES} )
  

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
  
  ########
  # Add an unity-test, if available
  ########
  if( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
    add_subdirectory("test")
  endif()
  
endif()