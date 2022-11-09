################################################################################
# Create Tests only if we are building CcOS
################################################################################
if("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")

  set ( CURRENT_PROJECT ${CURRENT_PROJECT}Test )

  ########
  # Add Source Files
  ########
  set (SOURCE_FILES
        "main.cpp"
        "CServerTest.cpp"
        "CServerTest.h"
        "CClientTest.cpp"
        "CClientTest.h")

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/Testing/${CURRENT_PROJECT_IDE_PATH}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                                CcScpi
                                CcTesting )

  CcAddTest(${CURRENT_PROJECT})

  #################
  # Post configuration steps
  #################
  CcOSExeSettings(${CURRENT_PROJECT})
  CcSetFiltersByFolders(${SOURCE_FILES})

endif("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")
