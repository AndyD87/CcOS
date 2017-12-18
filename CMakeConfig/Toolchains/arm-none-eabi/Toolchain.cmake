if( WIN32 )
  # this one is important
  set( WORKING_DIR "@CCOS_DIR@/CMakeConfig/Toolchains/arm-none-eabi" )
  set( GCC_DIR "${WORKING_DIR}/arm-none-eabi" )
  
  if(NOT EXISTS ${GCC_DIR})
    MESSAGE("- Download gcc")
    file(DOWNLOAD http://mirror.adirmeier.de/binaries/Toolchains/gcc-arm-none-eabi-6-2017-q1-update-win32.zip ${WORKING_DIR}/gcc-arm-none-eabi.zip)
    
    file(MAKE_DIRECTORY ${GCC_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${WORKING_DIR}/gcc-arm-none-eabi.zip
                    WORKING_DIRECTORY                  ${GCC_DIR}/)
  endif()
  
  SET(CMAKE_SYSTEM_NAME       Generic)
  SET(CMAKE_SYSTEM_PROCESSOR  arm)
  
  SET(CMAKE_CROSSCOMPILING 1)
  
  # specify the cross compiler
  SET(CMAKE_ASM_COMPILER ${GCC_DIR}/bin/arm-none-eabi-as)
  SET(CMAKE_C_COMPILER   ${GCC_DIR}/bin/arm-none-eabi-gcc.exe)
  SET(CMAKE_CXX_COMPILER ${GCC_DIR}/bin/arm-none-eabi-g++.exe)
  SET(CMAKE_LINKER       ${GCC_DIR}/bin/arm-none-eabi-ld.exe)
  
  # search for programs in the build host directories
  SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
  # for libraries and headers in the target directories
  SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
  SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
  SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
else()

endif()
