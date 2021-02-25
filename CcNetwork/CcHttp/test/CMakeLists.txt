################################################################################
# Create Tests only if we are building CcOS
################################################################################
if("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")

  set ( CURRENT_PROJECT CcHttpTest )

  ########
  # Add Source Files
  ########
  set (SOURCE_FILES
        "main.cpp"
        "CHttpServerTest.cpp"
        "CHttpServerTest.h"
        "CClientTest.cpp"
        "CClientTest.h")

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  set(HttpTestEnvironment_FILENAME      "HttpTestEnvironment-win-x64.7z" )
  set(HttpTestEnvironment_URL           "https://coolcow.de/projects/Tools/HttpTestEnvironment/binaries/1.0.0.0/${HttpTestEnvironment_FILENAME}")
  set(HttpTestEnvironment_PATH          "${CC_CACHE_DIR}/Tools/HttpTestEnvironment/1.0.0.0" )

  CcDownloadAndExtract( "${HttpTestEnvironment_FILENAME}"
                        "${HttpTestEnvironment_PATH}"
                        "${HttpTestEnvironment_URL}"
  )

  add_definitions(-DHttpTestEnvironment_PATH="${HttpTestEnvironment_PATH}")
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
    add_definitions(-DHttpTestEnvironment_nginx="${HttpTestEnvironment_PATH}/nginx/nginx.exe")
  elseif(LINUX)
    find_program(HttpTestEnvironment_nginx nginx)
    add_definitions(-DHttpTestEnvironment_nginx="${HttpTestEnvironment_nginx}")
  endif()

  CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/Testing/CcNetwork")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                                CcHttp
                                CcTesting )

  CcAddTest(${CURRENT_PROJECT})

  #################
  # Post configuration steps
  #################
  CcOSExeSettings(${CURRENT_PROJECT})
  CcSetFiltersByFolders(${SOURCE_FILES})

endif("${CMAKE_PROJECT_NAME}" STREQUAL "CcOS")
