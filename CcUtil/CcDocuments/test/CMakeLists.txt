################################################################################
# Create Tests only if we are building CcOS
################################################################################
if("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")

  set(CURRENT_PROJECT CcDocumentsTest)

  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.c"
        "*.cpp"
        "*.h")
  
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT})
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
  endif()
  
  add_executable( ${CURRENT_PROJECT} ${SOURCE_FILES} )
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/Testing/CcUtil")
  
  source_group( "" FILES ${SOURCE_FILES})
  
  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                                CcDocuments
                                CcKernel )
  CcAddTest( ${CURRENT_PROJECT} )
             
endif("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")