if( WIN32 )
  message(FATAL_ERROR "not running in windows")
else()
  # this one is important
  set( WORKING_DIR "${CC_CACHE_DIR}/Toolchains" )
  set( GCC_DIR      "${WORKING_DIR}/esp-open-sdk" )
  
  CcDownloadAndExtract( "esp-open-sdk" 
                        "${GCC_DIR}"
                        "http://coolcow.de/projects/ThirdParty/esp-open-sdk/binaries/2.0.0.0/Ubuntu-x64.tar.gz")

  # specify the cross compiler
  set(CMAKE_ASM_COMPILER ${GCC_DIR}/xtensa-lx106-elf/bin/xtensa-lx106-elf-as     CACHE INTERNAL "")
  set(CMAKE_C_COMPILER   ${GCC_DIR}/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc    CACHE INTERNAL "")
  set(CMAKE_CXX_COMPILER ${GCC_DIR}/xtensa-lx106-elf/bin/xtensa-lx106-elf-g++    CACHE INTERNAL "")
  set(CMAKE_AR           ${GCC_DIR}/xtensa-lx106-elf/bin/xtensa-lx106-elf-ar     CACHE INTERNAL "")
  set(GCC_SIZE           ${GCC_DIR}/xtensa-lx106-elf/bin/xtensa-lx106-elf-size   CACHE INTERNAL "")
  set(ENV{PATH}          $ENV{PATH}:${GCC_DIR}/xtensa-lx106-elf/bin)
  
  # search for programs in the build host directories
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER             CACHE INTERNAL "")
  # for libraries and headers in the target directories
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY              CACHE INTERNAL "")
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY              CACHE INTERNAL "")
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY              CACHE INTERNAL "")
endif()

set(CMAKE_SYSTEM_NAME       Generic   CACHE INTERNAL "")
set(CMAKE_SYSTEM_PROCESSOR  arm       CACHE INTERNAL "")

set(CMAKE_CROSSCOMPILING 1)
  
set(CC_UINT32_SIZET_CONFLICT    TRUE   CACHE INTERNAL "")
set(CC_LINK_TYPE                STATIC CACHE INTERNAL "")
set(GENERIC                     TRUE   CACHE INTERNAL "")

enable_language(ASM)

set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> ${ASM_SYS_FLAGS} -o <OBJECT> <SOURCE>"   CACHE INTERNAL "")
