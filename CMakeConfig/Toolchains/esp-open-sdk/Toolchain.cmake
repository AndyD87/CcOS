# this one is important
set( WORKING_DIR "${CC_CACHE_DIR}/Toolchains" )
set( GCC_DIR      "${WORKING_DIR}/esp-open-sdk/1.22.0.0" )

if(WIN32)
  message(FATAL_ERROR "not running in windows")
else()
  CcDownloadAndExtract( "esp-open-sdk"
                        "${GCC_DIR}"
                        "https://dl.espressif.com/dl/xtensa-lx106-elf-linux64-1.22.0-92-g8facf4c-5.2.0.tar.gz"
                        "https://coolcow.de/projects/ThirdParty/esp-open-sdk/binaries/1.22.0.0/Ubuntu-x64.tar.gz")

  set(ESP8266_RTOS_SDK_DIR  ${CC_CACHE_DIR}/Sources/espressif/ESP8266_RTOS)
  if(NOT EXISTS ${ESP8266_RTOS_SDK_DIR})
    CcGitClone(${ESP8266_RTOS_SDK_DIR} https://github.com/espressif/ESP8266_RTOS_SDK.git
                                        https://coolcow.de/projects/ThirdParty/Backup/ESP8266_RTOS_SDK.git)
    CcGitCheckout(${ESP8266_RTOS_SDK_DIR} "v3.2" )
    CcGitPatchApply(${ESP8266_RTOS_SDK_DIR} ${CMAKE_CURRENT_LIST_DIR}/ESP8266_RTOS.patch )
  endif()
  # Cleanup path, remove .. etc
  get_filename_component(ESP8266_RTOS_SDK_DIR ${ESP8266_RTOS_SDK_DIR} ABSOLUTE)

  set(ESP8266_RTOS_SDK_DIR ${ESP8266_RTOS_SDK_DIR}                    CACHE INTERNAL "")
  set(ENV{PATH}           $ENV{PATH}:${GCC_DIR}/xtensa-lx106-elf/bin  )
  set(ENV{IDF_PATH}       ${ESP8266_RTOS_SDK_DIR}                     )

  # specify the cross compiler if not already defined
  if(NOT CMAKE_ASM_COMPILER)
    set(CMAKE_ASM_COMPILER ${GCC_DIR}/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc   CACHE INTERNAL "")
  endif()
  if(NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER   ${GCC_DIR}/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc   CACHE INTERNAL "")
  endif()
  if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER ${GCC_DIR}/xtensa-lx106-elf/bin/xtensa-lx106-elf-g++   CACHE INTERNAL "")
  endif()
  set(CMAKE_AR           ${GCC_DIR}/xtensa-lx106-elf/bin/xtensa-lx106-elf-ar      CACHE INTERNAL "")
  set(GCC_SIZE           ${GCC_DIR}/xtensa-lx106-elf/bin/xtensa-lx106-elf-size    CACHE INTERNAL "")

  set(CCOS_ELF_TO_IMAGE_EXECUTABLE ${GCC_DIR}/esptool/esptool.py                  CACHE INTERNAL "")

  include_directories()
endif()

find_program(PYTHON_SEARCH "python3")
IF(PYTHON_SEARCH)
  set(PYTHON "python3")
else()
  set(PYTHON "python")
ENDIF()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER             CACHE INTERNAL "")
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY              CACHE INTERNAL "")
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY              CACHE INTERNAL "")
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY              CACHE INTERNAL "")

################################################################################
# Convert output elf to hex
################################################################################
macro( CcElfToImage Project)
  if(CCOS_ELF_TO_IMAGE_EXECUTABLE)
    add_custom_command(TARGET ${Project} POST_BUILD
            COMMENT "Invoking: Convert elf to hex"
            COMMAND ${CMAKE_COMMAND} -E echo ""
            COMMAND ${CMAKE_COMMAND} -E echo "Size of target hex file: "
            COMMAND ${CMAKE_COMMAND} -E echo ""
            COMMAND ${CMAKE_COMMAND} -E env "PATH=$ENV{PATH}"
                    python ${CCOS_ELF_TO_IMAGE_EXECUTABLE} elf2image "$<TARGET_FILE:${Project}>"
            COMMAND ${CMAKE_COMMAND} -E echo "")
  endif(CCOS_ELF_TO_IMAGE_EXECUTABLE)
endmacro()
