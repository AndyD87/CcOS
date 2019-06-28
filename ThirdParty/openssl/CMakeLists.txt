if(NOT DEFINED CCOS_THIRDPARTY_OPENSSL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_THIRDPARTY_OPENSSL_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  if( WINDOWS )
    if("${CCOS_THIRDPARTY_OPENSSL_PREBUILD}" STREQUAL true)
      cmake_minimum_required (VERSION 3.0)
      set(OPENSSL_VERSION ${CCOS_THIRDPARTY_OPENSSL_VERSION})
  
      if("${CCOS_THIRDPARTY_OPENSSL_STATIC}" STREQUAL true)
        set(OPENSSL_STATIC_TYPE STATIC)
      else()
        set(OPENSSL_STATIC_TYPE SHARED)
      endif()
  
      if("${CCOS_THIRDPARTY_OPENSSL_STATICRUNTIME}" STREQUAL true)
        set(OPENSSL_STATICRUNTIME_TYPE STATIC)
      else()
        set(OPENSSL_STATICRUNTIME_TYPE SHARED)
      endif()

      # OutputString DebugRelease StaticShared StaticSharedRuntime
      CcVisualStudioPostFix(VS_POSTFIX RELEASE ${OPENSSL_STATIC_TYPE} ${OPENSSL_STATICRUNTIME_TYPE})
  
      # setup varibles for command
      set(OPENSSL_ZIP_FILENAME  "openssl-${VS_POSTFIX}.7z" )
      set(DOWNLOAD_URL          "http://coolcow.de/projects/ThirdParty/Openssl/binaries/${OPENSSL_VERSION}/${OPENSSL_ZIP_FILENAME}")
      set(OPENSSL_FILENAME      "openssl-${OPENSSL_VERSION}-${VS_POSTFIX}")
      set(OPENSSL_PATH          ${CC_CACHE_DIR}/${OPENSSL_FILENAME} )
      set(OPENSLL_INCLUDE_DIR   ${OPENSSL_PATH}/include )
      set(OPENSLL_LIB_DIR       ${OPENSSL_PATH}/lib     )

      
      CcDownloadAndExtract( "${OPENSSL_FILENAME}" 
                            "${OPENSSL_PATH}"
                            "${DOWNLOAD_URL}")
      
      add_custom_target(  openssl ALL )

      set_property( TARGET openssl PROPERTY FOLDER "CcOS/ThirdParty")

      # export variables to parent
      set(OPENSSL_LIBRARIES    ${OPENSLL_LIB_DIR}/libssl.lib ${OPENSLL_LIB_DIR}/libcrypto.lib Crypt32.lib  PARENT_SCOPE)
      set(OPENSLL_INCLUDE_DIR  ${OPENSLL_INCLUDE_DIR}        PARENT_SCOPE)
      set(OPENSLL_LIB_DIR      ${OPENSLL_LIB_DIR}            PARENT_SCOPE)
      
      if(GCC)
        list(APPEND OPENSSL_LIBRARIES "msvcrt"  PARENT_SCOPE)
      endif()
    else()
      message(FATAL_ERROR "building openssl currently not supported")
    endif("${CCOS_THIRDPARTY_OPENSSL_PREBUILD}" STREQUAL true)
  elseif( LINUX )
    find_package(OpenSSL QUIET)
    if( OPENSSL_FOUND )
      message("- OpenSsl found")
      set( OPENSLL_INCLUDE_DIR ${OPENSLL_INCLUDE_DIR} PARENT_SCOPE)
      set( OPENSSL_LIB_DIR     ${OPENSSL_LIB_DIR}     PARENT_SCOPE)
      set( OPENSSL_LIBRARIES   ${OPENSSL_LIBRARIES}   PARENT_SCOPE)
    else()
      message( FATAL_ERROR "SSL marked as required, but not found on system. Please install libssl-dev" )
    endif()
  else()
      message(FATAL_ERROR "OpenSsl is marked as required, but currently not available on your platform")
  endif()
endif()