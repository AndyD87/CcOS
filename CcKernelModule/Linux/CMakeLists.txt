################################################################################
#
# Prebuild-Step
#   apt-get install build-essential linux-headers-`uname -r`
# Install-Step
#   sudo insmod CcKernelModuleLinux.ko
# Uninstall-Step
#   sudo rmmod CcKernelModuleLinux.ko
################################################################################

configure_file( "${CMAKE_CURRENT_LIST_DIR}/Kbuild.in"
                "${CMAKE_CURRENT_LIST_DIR}/Kbuild" @ONLY)

file (GLOB SOURCE_FILES
      "*.c"
      "*.h"
      "Kbuild"
      "Kbuild.in"
)

add_custom_command( OUTPUT CcKernelModule.ko
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CCKERNELMODULE_OBJECT} ${CMAKE_CURRENT_BINARY_DIR}
                      COMMAND ${CMAKE_MAKE_PROGRAM} -C ${KERNELHEADERS_DIR} M=${CMAKE_CURRENT_BINARY_DIR} src=${CMAKE_CURRENT_SOURCE_DIR} modules
                      DEPENDS ${SOURCE_FILES}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                      COMMENT "Building kernel module..."
)

add_custom_target ( CcKernelModuleLinux ALL DEPENDS CcKernelModule.ko
                    SOURCES ${SOURCE_FILES}
)
