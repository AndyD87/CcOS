
MESSAGE("OpenSsl search")

if( MSVC )
  if( MSVC12 )
    SET( MSVCNAME "msvc120")
  elseif ( MSVC14 )
    SET( MSVCNAME "msvc140")
  else()
    SET( MSVCNAME "msvc140")
  endif()
  
  if(${CCOS_BUILD_TYPE} STREQUAL "DEBUG")
    set( CCOS_OPENSSL_BUILD_TYPE debug )
  else()
    set( CCOS_OPENSSL_BUILD_TYPE release )
  endif()

  MESSAGE("  Download OpenSsl for (${MSVCNAME}) to binary directory ")
  if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
    file(DOWNLOAD http://mirror.adirmeier.de/libraries/openssl.1.1.0e.${MSVCNAME}.x64.zip ${CMAKE_CURRENT_SOURCE_DIR}/openssl.zip)
    SET(OPENSSL_ZIP_FILE    ${CMAKE_CURRENT_SOURCE_DIR}/openssl.zip )
    SET(OPENSLL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/openssl.${MSVCNAME}.x64.${CCOS_OPENSSL_BUILD_TYPE}.static/include )
    SET(OPENSLL_LIB_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/openssl.${MSVCNAME}.x64.${CCOS_OPENSSL_BUILD_TYPE}.static/lib     )
  else()
    file(DOWNLOAD http://mirror.adirmeier.de/libraries/openssl.1.1.0e.${MSVCNAME}.x86.zip ${CMAKE_CURRENT_SOURCE_DIR}/openssl.zip)
    SET(OPENSSL_ZIP_FILE    ${CMAKE_CURRENT_SOURCE_DIR}/openssl.zip )
    SET(OPENSLL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/openssl.${MSVCNAME}.x86.${CCOS_OPENSSL_BUILD_TYPE}.static/include )
    SET(OPENSLL_LIB_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/openssl.${MSVCNAME}.x86.${CCOS_OPENSSL_BUILD_TYPE}.static/lib     )
  endif()

  Add_Custom_Command( OUTPUT  ${OPENSLL_LIB_DIR}/libssl.lib
                       COMMAND ${CMAKE_COMMAND} -E tar xf ${OPENSSL_ZIP_FILE}
                       DEPENDS ${OPENSSL_ZIP_FILE}
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

  add_custom_target(  openssl 
                      ALL
                      DEPENDS
                        ${OPENSLL_LIB_DIR}/libssl.lib )

  # export variables to parent
  set(OPENSSL_LIBRARIES libssl libcrypto Crypt32.lib  PARENT_SCOPE)
  set(OPENSLL_INCLUDE_DIR  ${OPENSLL_INCLUDE_DIR}     PARENT_SCOPE)
  set(OPENSLL_LIB_DIR      ${OPENSLL_LIB_DIR}         PARENT_SCOPE)
                    
  link_directories(${OPENSLL_LIB_DIR})
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
endif()
