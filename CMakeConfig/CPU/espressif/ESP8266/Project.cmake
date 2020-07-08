########
# Make this project dir as source directory
########
set(TEMP_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

########
# Load components
########
cmake_minimum_required(VERSION 3.5)
if(ESP8266_RTOS_SDK_DIR)
  include($ENV{IDF_PATH}/tools/cmake/project.cmake)

  # ESP8266 sdk has an implementation of main, we have to remove that
  add_definitions(-Dmain=main_override -DCCOS_MAIN_REPLACED)

  message($ENV{IDF_PATH})
  message($ENV{PATH})

  set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )
  set(SDKCONFIG_DEFAULTS "${CMAKE_CURRENT_LIST_DIR}/sdkconfig.defaults" )

  project(CcOS)
elseif()
  message(FATAL_ERROR "ESP8266 SDK not found")
endif()

########
# Restore source directory
########
set(CMAKE_SOURCE_DIR ${TEMP_SOURCE_DIR})
