if(NOT DEFINED CCOS_THIRDPARTY_SQLITE3_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_THIRDPARTY_SQLITE3_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set(CURRENT_PROJECT wiringPi)

  # Search for installed wiring Pi
  find_package ( wiringPi QUIET )

  if(WIRINGPI_FOUND)

  else()
    set(WRINIGPI_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/source)


    # Wiring pi not found download and add for build
    # Defined board type is required.
    if(NOT CCOS_BOARD_TYPE)
      message(FATAL_ERROR "Wiring Pi requires defined board type ${CCOS_BOARD_TYPE}")
    elseif(${CCOS_BOARD_TYPE} STREQUAL "ORANGEPI_ZERO")
      CcGitClone(${WRINIGPI_SOURCE_DIR} https://github.com/orangepi-xunlong/wiringOP.git
                                        https://coolcow.de/projects/ThirdParty/wiringOP.git
      )

      if(ZeroMQ_FOUND)

        add_library(CcOS::ThirdParty::wiringPi )


      endif()

    endif()

  endif()



endif()
