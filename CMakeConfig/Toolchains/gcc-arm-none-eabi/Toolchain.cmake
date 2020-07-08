if( WIN32 )
  # this one is important
  set( WORKING_DIR "${CC_CACHE_DIR}/Toolchains" )
  set( GCC_DIR      "${WORKING_DIR}/gcc-arm-none-eabi" )

  CcDownloadAndExtract( "gcc-arm-none-eabi"
                        "${GCC_DIR}"
                        "http://coolcow.de/projects/ThirdParty/gcc-arm-none-eabi/binaries/8.2018.4/gcc-arm-none-eabi_windows.zip")
  CcLoadMakeProgram()

  # specify the cross compiler
  set(CMAKE_ASM_COMPILER ${GCC_DIR}/bin/arm-none-eabi-gcc.exe    )
  set(CMAKE_C_COMPILER   ${GCC_DIR}/bin/arm-none-eabi-gcc.exe    )
  set(CMAKE_CXX_COMPILER ${GCC_DIR}/bin/arm-none-eabi-g++.exe    )
  set(CMAKE_LINKER       ${GCC_DIR}/bin/arm-none-eabi-gcc.exe    )
  set(CMAKE_AR           ${GCC_DIR}/bin/arm-none-eabi-ar.exe     )
  set(GCC_SIZE           ${GCC_DIR}/bin/arm-none-eabi-size.exe   )

  # search for programs in the build host directories
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER             )
  # for libraries and headers in the target directories
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY              )
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY              )
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY              )
else()
  # this one is important
  set( WORKING_DIR "${CC_CACHE_DIR}/Toolchains" )
  set( GCC_DIR      "${WORKING_DIR}/gcc-arm-none-eabi" )

  CcDownloadAndExtract( "gcc-arm-none-eabi"
                        "${GCC_DIR}"
                        "http://coolcow.de/projects/ThirdParty/gcc-arm-none-eabi/binaries/8.2018.4/gcc-arm-none-eabi_linux.tar.bz2")

  # specify the cross compiler
  set(CMAKE_ASM_COMPILER ${GCC_DIR}/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-gcc    )
  set(CMAKE_C_COMPILER   ${GCC_DIR}/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-gcc    )
  set(CMAKE_CXX_COMPILER ${GCC_DIR}/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-g++    )
  set(CMAKE_AR           ${GCC_DIR}/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-ar     )
  set(GCC_SIZE           ${GCC_DIR}/gcc-arm-none-eabi-8-2018-q4-major/bin/arm-none-eabi-size   )

  # search for programs in the build host directories
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER             )
  # for libraries and headers in the target directories
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY              )
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY              )
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY              )
endif()

set(CMAKE_SYSTEM_NAME       Generic   )
set(CMAKE_SYSTEM_PROCESSOR  arm       )
set(CMAKE_CROSSCOMPILING    1         )

set(CC_LINK_TYPE STATIC )
set(GENERIC      TRUE   )

set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> -x assembler-with-cpp <FLAGS> -o <OBJECT> -c <SOURCE>"   )
