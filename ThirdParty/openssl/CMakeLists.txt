if( MSVC )
  set(OPENSSL_VERSION "1.1.0.4")
  # OutputString DebugRelease StaticShared StaticSharedRuntime
  CcOSVisualStudioPostFix(VS_POSTFIX RELEASE STATIC STATIC)
  
  set(OPENSSL_FILENAME "openssl-${VS_POSTFIX}")
  set(OPENSSL_ZIP_FILENAME "${OPENSSL_FILENAME}.7z")
  set(DOWNLOAD_URL "http://mirror.adirmeier.de/projects/ThirdParty/Openssl/binaries/${OPENSSL_VERSION}/${OPENSSL_ZIP_FILENAME}")
  
  if( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_ZIP_FILENAME} )
    MESSAGE("Download prebuild openssl: ${DOWNLOAD_URL}")
    file( DOWNLOAD 
            ${DOWNLOAD_URL} 
            ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_ZIP_FILENAME}
          STATUS DOWNLOAD_STATUS)
    LIST(GET DOWNLOAD_STATUS 0 NUMERIC_STATUS)
    if(NOT ${NUMERIC_STATUS} EQUAL 0)
      file(REMOVE ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_ZIP_FILENAME})
      MESSAGE(FATAL_ERROR "Download result: ${DOWNLOAD_STATUS}")
    else()
      MESSAGE("Download succeeded")
    endif()
  endif()
  
  # setup varibles for command
  set(OPENSSL_ZIP_FILE    ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_ZIP_FILENAME} )
  set(OPENSLL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_FILENAME}/include )
  set(OPENSLL_LIB_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_FILENAME}/lib     )

  # Create directory
  Add_Custom_Command(  OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_FILENAME}
                       COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_FILENAME}
                       DEPENDS ${OPENSSL_ZIP_FILE}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
                    
  # Extract file   
  Add_Custom_Command(  OUTPUT  libssl.lib
                       COMMAND ${CMAKE_COMMAND} -E tar xf ${OPENSSL_ZIP_FILE}
                       DEPENDS ${OPENSSL_ZIP_FILE}
                               ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_FILENAME}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_FILENAME} )

  add_custom_target(  openssl
                      ALL
                      DEPENDS libssl.lib )

  # export variables to parent
  set(OPENSSL_LIBRARIES    libssl.lib libcrypto.lib Crypt32.lib  PARENT_SCOPE)
  set(OPENSLL_INCLUDE_DIR  ${OPENSLL_INCLUDE_DIR}        PARENT_SCOPE)
  set(OPENSLL_LIB_DIR      ${OPENSLL_LIB_DIR}            PARENT_SCOPE)
                    
  set_property( TARGET openssl PROPERTY FOLDER "CcOS/ThirdParty")
elseif( LINUX )
  find_package(OpenSSL QUIET)
  if( OPENSSL_FOUND )
    MESSAGE("OpenSsl found")
    set( OPENSLL_INCLUDE_DIR ${OPENSLL_INCLUDE_DIR} PARENT_SCOPE)
    set( OPENSSL_LIB_DIR     ${OPENSSL_LIB_DIR}     PARENT_SCOPE)
    set( OPENSSL_LIBRARIES   ${OPENSSL_LIBRARIES}   PARENT_SCOPE)
  else()
    if( ${USE_THIRDPARTY_OPENSSL} STREQUAL "r")
      message( FATAL_ERROR "SSL marked as required, but not found on system. Please install libssl-dev" )
    else()
      unset(USE_THIRDPARTY_OPENSSL)
      message( ERROR "SSL marked for installation, but not found on system. SSL removed from build" )
    endif()
  endif()
elseif(${USE_THIRDPARTY_OPENSSL} STREQUAL "r")
    message(FATAL_ERROR "OpenSsl is marked as required, but currently not available on your platform")
endif()
