################################################################################
# Create Tests only if we are building CcOS
################################################################################
if("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")

  set ( CURRENT_TEST_PROJECT ${CURRENT_PROJECT}Test )

  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_FILES
        "*.c"
        "*.cpp"
        "*.h")

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_TEST_PROJECT} SOURCE_FILES )
  endif()

  CcAddExecutable( ${CURRENT_TEST_PROJECT} ${SOURCE_FILES} )
  
  set_property( TARGET ${CURRENT_TEST_PROJECT} PROPERTY FOLDER "CcOS/Testing/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_TEST_PROJECT} LINK_PRIVATE
                                CcKernel
                                CcTesting )

  CcAddTest(${CURRENT_TEST_PROJECT})

  #################
  # Post configuration steps
  #################
  CcOSExeSettings(${CURRENT_PROJECT})
  CcSetFiltersByFolders(${SOURCE_FILES})

endif("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")
