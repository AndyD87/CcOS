message("- Crosscompiling Toolchain activated")

if(WIN32)

  message(FATAL_ERROR "Windows currently not supported")

else()

  set( CMAKE_SYSTEM_NAME  Linux )
  set( LINARO_GCC_VERSION  "4.8.2015.6" )
  set( LINARO_GCC_DIR "${CC_CACHE_DIR}/Toolchains/gcc-linaro-arm-linux-gnueabihf/${LINARO_GCC_VERSION}" )

  CcDownloadAndExtract( "linaro-gcc-arm-none-eabi"
                        "${LINARO_GCC_DIR}"
                        "https://releases.linaro.org/archive/15.06/components/toolchain/binaries/4.8/arm-linux-gnueabihf/gcc-linaro-4.8-2015.06-x86_64_arm-linux-gnueabihf.tar.xz"
                        "https://coolcow.de/projects/ThirdParty/gcc-linaro-arm-linux-gnueabihf/binaries/${LINARO_GCC_VERSION}/gcc-linaro-4.8-2015.06-x86_64_arm-linux-gnueabihf.tar.xz"
                        SHA1 "9e4495bfe35a88d8fc8f3ed7427fb25c5a23fd9c"
  )
  CcLoadMakeProgram()

  set( LINARO_GCC_BIN_DIR "${LINARO_GCC_DIR}/gcc-linaro-4.8-2015.06-x86_64_arm-linux-gnueabihf/bin" )
  # specify the cross compiler
  set(CMAKE_ASM_COMPILER ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-as     CACHE INTERNAL "")
  set(CMAKE_C_COMPILER   ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-gcc    CACHE INTERNAL "")
  set(CMAKE_CXX_COMPILER ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-g++    CACHE INTERNAL "")
  set(CMAKE_AR           ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-ar     CACHE INTERNAL "")
  set(GCC_SIZE           ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-size   CACHE INTERNAL "")

  # search for programs in the build host directories
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER             CACHE INTERNAL "")
  # for libraries and headers in the target directories
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY              CACHE INTERNAL "")
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY              CACHE INTERNAL "")
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY              CACHE INTERNAL "")

endif(WIN32)
