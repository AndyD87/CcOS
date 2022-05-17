################################################################################
# Create Tests only if we are building CcOS
################################################################################
if("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")

  set ( CURRENT_PROJECT CcAppInterfacesTest )

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

  CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/Testing/CcUtil")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                                CcKernel
                                CcAppInterfaces
                                CcTesting)

  CcAddTest(${CURRENT_PROJECT})

  #################
  # Post configuration steps
  #################
  CcOSExeSettings(${CURRENT_PROJECT})
  CcSetFiltersByFolders(${SOURCE_FILES})

endif("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")
