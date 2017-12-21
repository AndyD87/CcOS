################################################################################
# Create Tests only if we are building CcOS
################################################################################
if("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")
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
  
  add_executable( CcDocumentsTest ${SOURCE_FILES} )
  
  set_property( TARGET CcDocumentsTest PROPERTY FOLDER "CcOS/Testing/CcUtil")
  
  source_group( "" FILES ${SOURCE_FILES})
  
  target_link_libraries ( CcDocumentsTest LINK_PRIVATE
                                CcDocuments
                                CcKernel )
  add_test( NAME    CcDocumentTest
            COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CcDocumentsTest
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
             
endif("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")