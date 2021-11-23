message("- Setup orangepi zero board")
set(CCOS_BOARD_TYPE "ORANGEPI_ZERO")

if(EXISTS /etc/armbian-release)
    # We are on device, keep default settings
else()
    # Load linaro cross compiler
    include(${CMAKE_CURRENT_LIST_DIR}/../../../Toolchains/gcc-linaro-arm-linux-gnueabihf/Toolchain.cmake)

    # use sysroot from minimal build with
    # git clone https://github.com/armbian/build.git && cd build
    # echo "PACKAGE_LIST_ADDITIONAL=\"$PACKAGE_LIST_ADDITIONAL libssl-dev\"" > userpatches/lib.config
    # ./compile.sh \
    #   BOARD=orangepizero \
    #   BRANCH=current \
    #   RELEASE=focal \
    #   BUILD_MINIMAL=yes \
    #   BUILD_DESKTOP=no \
    #   KERNEL_ONLY=no \
    #   KERNEL_CONFIGURE=no
    #

    set(SYSROOT_DIR "${CC_CACHE_DIR}/Sysroot/Armbian" )
    CcDownloadAndExtract( "ArmbianSysroot"
                          "${SYSROOT_DIR}"
                          "https://coolcow.de/projects/ThirdParty/Armbian/binaries/0.0.0.1/focal-minimal-armhf.7efc0a5812b095319a8270f8c5f2bdd7.tar.lz4"
                          "f5c5236cdc3ada323b8db1f7ca02e7b62a6d56c6"
    )

    # Prepare Sysroot
    CcCopyDirectoryContent("${SYSROOT_DIR}/usr/include/arm-linux-gnueabihf" "${SYSROOT_DIR}/usr/include")
    CcCopyDirectoryContent("${SYSROOT_DIR}/usr/lib/arm-linux-gnueabihf" "${SYSROOT_DIR}/usr/lib")

    set(CMAKE_FIND_ROOT_PATH ${SYSROOT_DIR}/usr)
endif()
