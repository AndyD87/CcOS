###
# Setup Toolchain if required
###
if(NOT CMAKE_TOOLCHAIN_FILE)
    message("- Load toolchain: MinGW")
    include(${CMAKE_CURRENT_LIST_DIR}/../../Toolchains/MinGW/Toolchain.cmake)
endif()
