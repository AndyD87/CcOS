################################################################################
# Create Tests only if we are building CcOS
################################################################################
if("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")
  ##############################################################################
  # Add Source Files
  ##############################################################################
  file (GLOB SOURCE_FILES
        "*.c"
        "*.cpp"
        "*.h")
  
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT})
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
  endif()
  
  add_executable( CcKernelTest ${SOURCE_FILES} )
  
  set_property( TARGET CcKernelTest PROPERTY FOLDER "CcOS/Testing")
  
  SOURCE_GROUP( "" FILES ${SOURCE_FILES})
  
  target_link_libraries ( CcKernelTest LINK_PRIVATE CcKernel )
  add_test( NAME    CcKernelTest
            COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CcKernelTest
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
 
endif("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")