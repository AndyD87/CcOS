if(NOT DEFINED CCOS_THIRDPARTY_OPENSSL_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_THIRDPARTY_OPENSSL_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  if( WINDOWS )
    if("${CCOS_THIRDPARTY_OPENSSL_PREBUILD}" STREQUAL true)
      set(OPENSSL_VERSION ${CCOS_THIRDPARTY_OPENSSL_VERSION})

      if(DEFINED GCC)
        message("MinGW requires openssl to be shared ")
        set(OPENSSL_STATIC_TYPE SHARED)
      elseif("${CCOS_THIRDPARTY_OPENSSL_STATIC}" STREQUAL true)
        set(OPENSSL_STATIC_TYPE STATIC)
      else()
        set(OPENSSL_STATIC_TYPE ${CC_LINK_TYPE})
      endif()

      if("${CCOS_THIRDPARTY_OPENSSL_STATICRUNTIME}" STREQUAL true)
        set(OPENSSL_STATICRUNTIME_TYPE STATIC)
      else()
        set(OPENSSL_STATICRUNTIME_TYPE ${CC_LINK_TYPE})
      endif()

      # OutputString DebugRelease StaticShared StaticSharedRuntime
      CcVisualStudioPostFix(VS_POSTFIX RELEASE ${OPENSSL_STATIC_TYPE} ${OPENSSL_STATICRUNTIME_TYPE})

      # setup varibles for command
      set(OPENSSL_ZIP_FILENAME  "openssl-${VS_POSTFIX}.7z" )
      set(DOWNLOAD_URL          "https://coolcow.de/projects/ThirdParty/Openssl/binaries/${OPENSSL_VERSION}/${OPENSSL_ZIP_FILENAME}")
      set(OPENSSL_FILENAME      "openssl-${OPENSSL_VERSION}-${VS_POSTFIX}")
      set(OPENSSL_PATH          ${CC_CACHE_DIR}/Binaries/openssl/${OPENSSL_FILENAME} )
      set(OPENSLL_BIN_DIR       ${OPENSSL_PATH}/bin )
      set(OPENSSL_INCLUDE_DIR   ${OPENSSL_PATH}/include )
      set(OPENSLL_LIB_DIR       ${OPENSSL_PATH}/lib     )


      CcDownloadAndExtract( "${OPENSSL_FILENAME}"
                            "${OPENSSL_PATH}"
                            "${DOWNLOAD_URL}")

      # export variables to parent
      set(OPENSSL_LIBRARIES    ${OPENSLL_LIB_DIR}/libssl.lib ${OPENSLL_LIB_DIR}/libcrypto.lib crypt32.lib)

      message("- OpenSsl found")
      add_library(CcThirdPartySsl INTERFACE)
      target_include_directories(CcThirdPartySsl INTERFACE 
        $<BUILD_INTERFACE:${OPENSSL_INCLUDE_DIR}>
      )
      target_link_libraries     (CcThirdPartySsl INTERFACE ${OPENSSL_LIBRARIES})
      add_library(CcOS::ThirParty::ssl ALIAS CcThirdPartySsl)
      CcTargetInstall(CcThirdPartySsl)
      
      #message(FATAL_ERROR "${OPENSSL_STATIC_TYPE}")

      if("${OPENSSL_STATIC_TYPE}" STREQUAL SHARED)
        if(DEFINED WINDOWS)
          add_custom_target(_ssl_copy ALL)

          add_dependencies(CcThirdPartySsl _ssl_copy)
          
          set_property( TARGET _ssl_copy PROPERTY FOLDER "CcOS/ThirdParty")

          file (GLOB OPENSLL_BIN_FILES "${OPENSLL_BIN_DIR}/*.dll")
          foreach(OPENSLL_BIN_FILE ${OPENSLL_BIN_FILES})
            add_custom_command(TARGET _ssl_copy POST_BUILD
                                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                                "${OPENSLL_BIN_FILE}"
                                "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
          endforeach()
        endif()
      endif()

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
      add_library(CcThirdPartySsl INTERFACE)
      target_include_directories(CcThirdPartySsl INTERFACE ${OPENSSL_INCLUDE_DIR})
      target_link_libraries     (CcThirdPartySsl INTERFACE ${OPENSSL_LIBRARIES})
      add_library(CcOS::ThirParty::ssl ALIAS CcThirdPartySsl)
      CcTargetInstall(CcThirdPartySsl)
    else()
      message( FATAL_ERROR "SSL marked as required, but not found on system. Please install libssl-dev" )
    endif()
  else()
      message(FATAL_ERROR "OpenSsl is marked as required, but currently not available on your platform")
  endif()
endif()
