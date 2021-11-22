if(NOT DEFINED CCOS_THIRDPARTY_WIRINGPI_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_THIRDPARTY_WIRINGPI_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set(CURRENT_PROJECT wiringPi)

  set(WRINIGPI_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/source)

  # Search for installed wiring Pi
  find_package ( wiringPi QUIET )

  if(WIRINGPI_FOUND)
    add_library(CcOS::ThirdParty::wiringPi ALIAS wiringPi)
  else()

    # Wiring pi not found download and add for build
    # Defined board type is required.
    if(NOT CCOS_BOARD_TYPE)
      message(FATAL_ERROR "Wiring Pi requires defined board type ${CCOS_BOARD_TYPE}")
    elseif(${CCOS_BOARD_TYPE} STREQUAL "ORANGEPI_ZERO")
      add_definitions(-DCONFIG_ORANGEPI_ZERO)
      include (OrangePi.cmake)
      add_library(CcOS::ThirdParty::wiringPi ALIAS wiringPi)
    else()
      message(FATAL_ERROR "Wiring Pi requires defined board type ${CCOS_BOARD_TYPE}")
    endif()
  endif()
endif()
