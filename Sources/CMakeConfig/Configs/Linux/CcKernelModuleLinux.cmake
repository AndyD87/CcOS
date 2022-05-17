################################################################################
#
# Prebuild-Step
#   apt-get install build-essential linux-headers-`uname -r`
# Install-Step
#   sudo insmod CcKernelModuleLinux.ko
# Uninstall-Step
#   sudo rmmod CcKernelModuleLinux.ko
################################################################################

# Generate relative file path for kernel link
file(RELATIVE_PATH LINUX_KERNEL_FILE_PATH ${CMAKE_CURRENT_BINARY_DIR} "${CMAKE_CURRENT_LIST_DIR}/../../../CcKernelModule/Linux/main.c")
get_filename_component(LINUX_KERNEL_FILE_PATH ${LINUX_KERNEL_FILE_PATH} DIRECTORY)

configure_file( "${CMAKE_CURRENT_LIST_DIR}/Kbuild.in"
                "${CMAKE_CURRENT_BINARY_DIR}/Kbuild" @ONLY)

set (SOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/../../../CcKernelModule/Linux/main.c"
      "${CMAKE_CURRENT_LIST_DIR}/../../../CcKernelModule/Linux/CcMalloc.c"
      "${CMAKE_CURRENT_BINARY_DIR}/Kbuild"
)

set(VERBOSE_OUTPUT "V=1")

set(ProjectName_OBJECT "/" CACHE INTERNAL "")

set(ProjectName_OBJECT_PATH "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"                  CACHE INTERNAL "")
set(ProjectName_OBJECT_NAME "lib${CURRENT_PROJECT}.a"                            CACHE INTERNAL "")
set(ProjectName_OBJECT      "${ProjectName_OBJECT_PATH}/${ProjectName_OBJECT_NAME}"   CACHE INTERNAL "")

file(WRITE  ${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}MakeFile "all:\n")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}MakeFile "\tmake -C ${KERNELHEADERS_DIR} M=${CMAKE_CURRENT_BINARY_DIR} modules\n")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}MakeFile "clean:\n")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}MakeFile "\tmake -C ${KERNELHEADERS_DIR} M=${CMAKE_CURRENT_BINARY_DIR} clean\n")
file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}MakeFile "\n")

add_custom_command( OUTPUT ${ProjectName}.ko
                    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CCKERNELMODULE_OBJECT_NAME}.o
                    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${ProjectName_OBJECT_NAME}.o
                    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Kbuild
                    DEPENDS ${SOURCE_FILES}
                    DEPENDS ${CCKERNELMODULE_OBJECT}
                    DEPENDS ${ProjectName_OBJECT}
                      COMMAND echo copy from "${CCKERNELMODULE_OBJECT}" to "${CMAKE_CURRENT_BINARY_DIR}"
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CCKERNELMODULE_OBJECT} ${CMAKE_CURRENT_BINARY_DIR}/${CCKERNELMODULE_OBJECT_NAME}.o
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${ProjectName_OBJECT} ${CMAKE_CURRENT_BINARY_DIR}/${ProjectName_OBJECT_NAME}.o
                      COMMAND ${CMAKE_MAKE_PROGRAM} ${VERBOSE_OUTPUT} -f ${ProjectName}MakeFile
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                      COMMENT "Building kernel module..."
)
add_custom_target ( ${ProjectName}Km ALL
                    DEPENDS ${ProjectName}.ko
                    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${CCKERNELMODULE_OBJECT_NAME}.o
                    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${ProjectName_OBJECT_NAME}.o
                    SOURCES ${SOURCE_FILES}
)

add_dependencies(${ProjectName}Km ${ProjectName})
