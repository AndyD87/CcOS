if( WIN32 )
  # this one is important
  set( WORKING_DIR "${CC_CACHE_DIR}/Toolchains" )
  set( GCC_DIR      "${WORKING_DIR}/gcc-arm-none-eabi" )
  
  CcDownloadAndExtract( "gcc-arm-none-eabi" 
                        "${GCC_DIR}"
                        "http://coolcow.de/projects/ThirdParty/gcc-arm-none-eabi/binaries/8.2018.4/gcc-arm-none-eabi_windows.zip")
  CcLoadMakeProgram()
  
  set(CMAKE_SYSTEM_NAME       Generic   PARENT_SCOPE)
  set(CMAKE_SYSTEM_PROCESSOR  arm       PARENT_SCOPE)
  
  set(CMAKE_CROSSCOMPILING 1)
  
  # specify the cross compiler
  set(CMAKE_ASM_COMPILER ${GCC_DIR}/bin/arm-none-eabi-as.exe     CACHE INTERNAL "")
  set(CMAKE_C_COMPILER   ${GCC_DIR}/bin/arm-none-eabi-gcc.exe    CACHE INTERNAL "")
  set(CMAKE_CXX_COMPILER ${GCC_DIR}/bin/arm-none-eabi-g++.exe    CACHE INTERNAL "")
  set(CMAKE_AR           ${GCC_DIR}/bin/arm-none-eabi-ar.exe     CACHE INTERNAL "")
  set(GCC_SIZE           ${GCC_DIR}/bin/arm-none-eabi-size.exe   CACHE INTERNAL "")
  
  # search for programs in the build host directories
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER             PARENT_SCOPE)
  # for libraries and headers in the target directories
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY              PARENT_SCOPE)
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY              PARENT_SCOPE)
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY              PARENT_SCOPE)
else()
  # this one is important
  set( WORKING_DIR "${CC_CACHE_DIR}/Toolchains" )
  set( GCC_DIR      "${WORKING_DIR}/gcc-arm-none-eabi" )
  
  CcDownloadAndExtract( "gcc-arm-none-eabi" 
                        "${GCC_DIR}"
                        "http://coolcow.de/projects/ThirdParty/gcc-arm-none-eabi/binaries/8.2018.4/gcc-arm-none-eabi_linux.tar.bz2")
  CcLoadMakeProgram()
  
  set(CMAKE_SYSTEM_NAME       Generic   PARENT_SCOPE)
  set(CMAKE_SYSTEM_PROCESSOR  arm       PARENT_SCOPE)
  
  set(CMAKE_CROSSCOMPILING 1)
  
  # specify the cross compiler
  set(CMAKE_ASM_COMPILER ${GCC_DIR}/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-as     CACHE INTERNAL "")
  set(CMAKE_C_COMPILER   ${GCC_DIR}/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-gcc    CACHE INTERNAL "")
  set(CMAKE_CXX_COMPILER ${GCC_DIR}/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-g++    CACHE INTERNAL "")
  set(CMAKE_AR           ${GCC_DIR}/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-ar     CACHE INTERNAL "")
  set(GCC_SIZE           ${GCC_DIR}/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-size   CACHE INTERNAL "")
  
  # search for programs in the build host directories
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER             PARENT_SCOPE)
  # for libraries and headers in the target directories
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY              PARENT_SCOPE)
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY              PARENT_SCOPE)
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY              PARENT_SCOPE)
endif()

set(CC_LINK_TYPE STATIC PARENT_SCOPE)
set(GENERIC)

# ST-Link generated Clock with 8MHz 
# If another clock is required replace this with remove_definitions
add_definitions(-DHSE_VALUE=8000000)