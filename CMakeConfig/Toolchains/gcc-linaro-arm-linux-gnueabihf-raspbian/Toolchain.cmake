message("- Crosscompiling Toolchain activated")
set( CMAKE_SYSTEM_NAME  Linux )
set( RASPBERRY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Toolchains/Raspberry" )
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    if( NOT EXISTS ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian-x64.tar.gz )
      file(DOWNLOAD https://github.com/AndyD87/Toolchains/raw/master/Raspberry/gcc-linaro-arm-linux-gnueabihf-raspbian-x64.tar.gz 
                    ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian-x64.tar.gz )
    endif()

    if( NOT EXISTS ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian-x64.tar.gz )
      file(DOWNLOAD http://adirmeier.de/git/Toolchains/gcc-linaro-arm-linux-gnueabihf-raspbian-x64.tar.gz 
                    ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian-x64.tar.gz )
    endif()

    if( EXISTS ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian-x64.tar.gz )
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf gcc-linaro-arm-linux-gnueabihf-raspbian-x64.tar.gz
                      WORKING_DIRECTORY                  ${RASPBERRY_DIR}/)
    
      set( CMAKE_C_COMPILER       ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc)
      set( CMAKE_CXX_COMPILER     ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++)
      set( CMAKE_FIND_ROOT_PATH   ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/)
    endif()
else()
    if( NOT EXISTS ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian.tar.gz )
      file(DOWNLOAD https://github.com/AndyD87/Toolchains/raw/master/Raspberry/gcc-linaro-arm-linux-gnueabihf-raspbian.tar.gz 
                    ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian.tar.gz )
    endif()

    if( NOT EXISTS ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian.tar.gz )
      file(DOWNLOAD http://adirmeier.de/git/Toolchains/gcc-linaro-arm-linux-gnueabihf-raspbian.tar.gz 
                    ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian.tar.gz )
    endif()

    if( EXISTS ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian.tar.gz )
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf gcc-linaro-arm-linux-gnueabihf-raspbian.tar.gz
                      WORKING_DIRECTORY                  ${RASPBERRY_DIR})
      set( CMAKE_C_COMPILER       ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc)
      set( CMAKE_CXX_COMPILER     ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++)
      set( CMAKE_FIND_ROOT_PATH   ${RASPBERRY_DIR}/gcc-linaro-arm-linux-gnueabihf-raspbian/)
    endif()
endif()
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

if( NOT EXISTS ${RASPBERRY_DIR}/userland.tar.gz )
  file(DOWNLOAD https://github.com/AndyD87/Toolchains/raw/master/Raspberry/userland.tar.gz 
                ${RASPBERRY_DIR}/userland.tar.gz )
endif()

if( NOT EXISTS ${RASPBERRY_DIR}/userland.tar.gz )
  file(DOWNLOAD http://adirmeier.de/git/Toolchains/userland.tar.gz 
                ${RASPBERRY_DIR}/userland.tar.gz )
endif()

execute_process(COMMAND tar xfvz userland.tar.gz
                WORKING_DIRECTORY ${RASPBERRY_DIR}/)
