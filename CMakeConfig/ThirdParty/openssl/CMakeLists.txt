
MESSAGE("OpenSsl search")

if( MSVC )
  # Get target visual studio version
  if( MSVC12 )
    SET( MSVCNAME "msvc120")
  elseif ( MSVC14 )
    SET( MSVCNAME "msvc140")
  else()
    # Correct visual studio version not found, use VS 2015
    SET( MSVCNAME "msvc140")
  endif()
  
  # Setup debug or release binaries
  if("${CCOS_BUILD_TYPE}" STREQUAL "DEBUG")
    set( CCOS_OPENSSL_BUILD_TYPE debug )
    message( "openssl-debug selected")
  else()
    set( CCOS_OPENSSL_BUILD_TYPE release )
    message( "openssl-release selected")
  endif()

  MESSAGE("  Download OpenSsl for (${MSVCNAME}) to binary directory ")

  # Set correct platoform type to filename
  if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
    SET( OPENSSL_PLATFORM_NAME x64)
    SET( OPENSSL_ZIP_FILENAME openssl.1.1.0e.${MSVCNAME}.${OPENSSL_PLATFORM_NAME}.zip)
  else()
    SET( OPENSSL_PLATFORM_NAME x86)
    SET(OPENSSL_ZIP_FILENAME openssl.1.1.0e.${MSVCNAME}.${OPENSSL_PLATFORM_NAME}.zip)
  endif()

  # Download file if not exits
  if( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_ZIP_FILENAME} )
    file(DOWNLOAD http://mirror.adirmeier.de/libraries/openssl/${OPENSSL_ZIP_FILENAME} ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_ZIP_FILENAME})
  endif()

  # setup varibles for command
  set(OPENSSL_ZIP_FILE    ${CMAKE_CURRENT_SOURCE_DIR}/${OPENSSL_ZIP_FILENAME} )
  set(OPENSLL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/openssl.${MSVCNAME}.${OPENSSL_PLATFORM_NAME}.${CCOS_OPENSSL_BUILD_TYPE}.static/include )
  set(OPENSLL_LIB_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/openssl.${MSVCNAME}.${OPENSSL_PLATFORM_NAME}.${CCOS_OPENSSL_BUILD_TYPE}.static/lib     )

  Add_Custom_Command(  OUTPUT  ${OPENSLL_LIB_DIR}/libssl.lib
                       COMMAND ${CMAKE_COMMAND} -E tar xf ${OPENSSL_ZIP_FILE}
                       DEPENDS ${OPENSSL_ZIP_FILE}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

  add_custom_target(  openssl
                      ALL
                      DEPENDS ${OPENSLL_LIB_DIR}/libssl.lib )

  # export variables to parent
  set(OPENSSL_LIBRARIES    libssl libcrypto Crypt32.lib  PARENT_SCOPE)
  set(OPENSLL_INCLUDE_DIR  ${OPENSLL_INCLUDE_DIR}     PARENT_SCOPE)
  set(OPENSLL_LIB_DIR      ${OPENSLL_LIB_DIR}         PARENT_SCOPE)
                    
  set_property( TARGET openssl PROPERTY FOLDER "ThirdParty")

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
