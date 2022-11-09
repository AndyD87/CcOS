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
        "CHttpServerTest.cpp"
        "CHttpServerTest.h"
        "CHttpServerWorkerTest.cpp"
        "CHttpServerWorkerTest.h"
        "CHeaderTest.cpp"
        "CHeaderTest.h"
        "CClientTest.cpp"
        "CClientTest.h")

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

  set(HttpTestEnvironment_VERSION       1.0.1.0 )
  set(HttpTestEnvironment_FILENAME      "HttpTestEnvironment-win-x64.7z" )
  set(HttpTestEnvironment_URL           "https://coolcow.de/projects/Tools/HttpTestEnvironment/binaries/${HttpTestEnvironment_VERSION}/${HttpTestEnvironment_FILENAME}")
  set(HttpTestEnvironment_PATH          "${CC_CACHE_DIR}/Tools/HttpTestEnvironment/${HttpTestEnvironment_VERSION}" )

  CcDownloadAndExtract( "${HttpTestEnvironment_FILENAME}"
                        "${HttpTestEnvironment_PATH}"
                        "${HttpTestEnvironment_URL}"
  )

  # Use nginx subdir as base path for nginx configuration
  add_definitions(-DHttpTestEnvironment_PATH="${HttpTestEnvironment_PATH}/nginx")
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
    add_definitions(-DHttpTestEnvironment_nginx="${HttpTestEnvironment_PATH}/nginx/nginx.exe")
    add_definitions(-DHttpTestEnvironment_phpcgi="${HttpTestEnvironment_PATH}/php/php-cgi.exe")
  elseif(LINUX_HTTP_ENV)
    find_program(HttpTestEnvironment_nginx nginx)
    add_definitions(-DHttpTestEnvironment_nginx="${HttpTestEnvironment_nginx}")
    find_program(HttpTestEnvironment php-cgi)
    message(${HttpTestEnvironment})
    add_definitions(-DHttpTestEnvironment_phpcgi="${HttpTestEnvironment}")
  endif()

  CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/Testing/${CURRENT_PROJECT_IDE_PATH}")

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
