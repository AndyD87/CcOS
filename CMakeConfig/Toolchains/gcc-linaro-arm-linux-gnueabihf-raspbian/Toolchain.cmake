message("- Crosscompiling Toolchain activated")

if(WIN32)

  message(FATAL_ERROR "Windows currently not supported")

else()

  set( CMAKE_SYSTEM_NAME  Linux )
  set( LINARO_GCC_DIR "${CC_CACHE_DIR}/Toolchains/gcc-linaro-arm-linux-gnueabihf" )

  CcDownloadAndExtract( "linaro-gcc-arm-none-eabi"
                        "${LINARO_GCC_DIR}"
                        "https://releases.linaro.org/archive/13.04/components/toolchain/binaries/gcc-linaro-arm-linux-gnueabihf-4.7-2013.04-20130415_linux.tar.xz")
  CcLoadMakeProgram()

  set( LINARO_GCC_BIN_DIR "${LINARO_GCC_DIR}/gcc-linaro-arm-linux-gnueabihf-4.7-2013.04-20130415_linux/bin" )
  # specify the cross compiler
  set(CMAKE_ASM_COMPILER ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-as     CACHE INTERNAL "")
  set(CMAKE_C_COMPILER   ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-gcc    CACHE INTERNAL "")
  set(CMAKE_CXX_COMPILER ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-g++    CACHE INTERNAL "")
  set(CMAKE_AR           ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-ar     CACHE INTERNAL "")
  set(GCC_SIZE           ${LINARO_GCC_BIN_DIR}/arm-linux-gnueabihf-size   CACHE INTERNAL "")

  # use sysroot from minimal build at
  #  https://github.com/armbian/build.git
  # echo "PACKAGE_LIST_ADDITIONAL=\"$PACKAGE_LIST_ADDITIONAL libssl-dev\"" > userpatches/lib.config
  #   ./compile.sh \
  #     BOARD=orangepizero \
  #     BRANCH=current \
  #     RELEASE=focal \
  #     BUILD_MINIMAL=yes \
  #     BUILD_DESKTOP=no \
  #     KERNEL_ONLY=no \
  #     KERNEL_CONFIGURE=no \
  #

  set(SYSROOT_DIR "${CC_CACHE_DIR}/Sysroot/Armbian" )
  CcDownloadAndExtract( "ArmbianSysroot"
                        "${SYSROOT_DIR}"
                        "https://coolcow.de/projects/ThirdParty/Armbian/binaries/0.0.0.1/focal-minimal-armhf.7efc0a5812b095319a8270f8c5f2bdd7.tar.lz4"
                        "f5c5236cdc3ada323b8db1f7ca02e7b62a6d56c6")

  foreach(TARGET  EXE SHARED STATIC MODULE)
  	set(CMAKE_${TARGET}_LINKER_FLAGS_INIT "-L\"${SYSROOT_DIR}/usr/lib/arm-linux-gnueabihf\"")
    set(CMAKE_${TARGET}_LINKER_FLAGS "-L\"${SYSROOT_DIR}/usr/lib/arm-linux-gnueabihf\"")
  	set(CMAKE_${TARGET}_LINKER_FLAGS_INIT "-L\"${SYSROOT_DIR}/usr/lib/arm-linux-gnueabihf\"" CACHE INTERNAL "")
  	set(CMAKE_${TARGET}_LINKER_FLAGS "-L\"${SYSROOT_DIR}/usr/lib/arm-linux-gnueabihf\"" CACHE INTERNAL "")
  endforeach()

  foreach(TARGET  EXE SHARED STATIC MODULE)
  	message(${CMAKE_${TARGET}_LINKER_FLAGS_INIT})
  endforeach()

  set(CMAKE_FIND_ROOT_PATH ${SYSROOT_DIR})

  # search for programs in the build host directories
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER             CACHE INTERNAL "")
  # for libraries and headers in the target directories
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY              CACHE INTERNAL "")
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY              CACHE INTERNAL "")
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY              CACHE INTERNAL "")

endif(WIN32)
