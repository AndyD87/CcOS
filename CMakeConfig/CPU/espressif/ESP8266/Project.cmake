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
  add_custom_command(OUTPUT Environment_Variables
                      COMMAND sh -c 'echo Setup Environment done'
                      COMMAND sh -c 'export IDF_PATH=$ENV{IDF_PATH}'
                      COMMAND sh -c 'export PATH=$ENV{PATH}:$$PATH'
                      COMMAND sh -c 'echo Setup Environment done' )
  add_custom_target("Environment_Variables" DEPENDS Environment_Variables )

  include($ENV{IDF_PATH}/tools/cmake/project.cmake)

  message($ENV{IDF_PATH})
  message($ENV{PATH})

  set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
  set(SDKCONFIG_DEFAULTS "${CMAKE_CURRENT_LIST_DIR}/sdkconfig.defaults" CACHE INTERNAL "")

  project(CcOS)
elseif()
  message(FATAL_ERROR "ESP8266 SDK not found")
endif()

########
# Restore source directory
########
set(CMAKE_SOURCE_DIR ${TEMP_SOURCE_DIR})
