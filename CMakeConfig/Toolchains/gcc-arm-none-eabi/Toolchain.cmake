if( WIN32 )
  # this one is important
  set( WORKING_DIR "${CC_CACHE_DIR}/Toolchains" )
  set( GCC_DIR      "${WORKING_DIR}/gcc-arm-none-eabi" )
  set( GCC_ZIP_FILE "${WORKING_DIR}/gcc-arm-none-eabi.zip")
  
  if(NOT EXISTS ${GCC_DIR})
    if(NOT EXISTS ${GCC_ZIP_FILE})
      message("- Download gcc")
      file( DOWNLOAD http://coolcow.de/projects/ThirdParty/gcc-arm-none-eabi/binaries/7.2018.2/gcc-arm-none-eabi.zip 
            ${GCC_ZIP_FILE}
            STATUS DOWNLOAD_STATUS)
      list(GET DOWNLOAD_STATUS 0 NUMERIC_STATUS)
      if(NOT ${NUMERIC_STATUS} EQUAL 0)
        file(REMOVE ${GCC_ZIP_FILE})
        message(FATAL_ERROR "- Download result: ${DOWNLOAD_STATUS}")
      else()
        message("- Download succeeded")
      endif()    
    endif()
    
    if(EXISTS ${GCC_ZIP_FILE})
      file(MAKE_DIRECTORY ${GCC_DIR})
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${GCC_ZIP_FILE}
                      WORKING_DIRECTORY                  ${GCC_DIR}
                      RESULT_VARIABLE GCC_ZIP_FILE_EXTRACT_RESULT)
      if(${GCC_ZIP_FILE_EXTRACT_RESULT} EQUAL 0)
        file(REMOVE ${GCC_ZIP_FILE})
      else()
        file(REMOVE ${GCC_ZIP_FILE})
        file(REMOVE_RECURSE ${GCC_DIR})
        message(FATAL_ERROR "- Extract error: ${GCC_ZIP_FILE_EXTRACT_RESULT}")
      endif()
    endif()
  else(NOT EXISTS ${GCC_DIR})
    message("- Download gcc not required: ${GCC_DIR}")       
  endif()
  
  if(NOT CMAKE_MAKE_PROGRAM)
    set( NINJA_DIR      "${WORKING_DIR}/Ninja" )
    set( NINJA_ZIP_FILE "${WORKING_DIR}/Ninja.zip")
    if(NOT EXISTS ${NINJA_DIR})
      if(NOT EXISTS ${NINJA_ZIP_FILE})
        message("- Download ninja")
        file( DOWNLOAD http://coolcow.de/projects/ThirdParty/Ninja/binaries/1.7.2.0/Ninja.zip 
              ${NINJA_ZIP_FILE}
              STATUS DOWNLOAD_STATUS)
        list(GET DOWNLOAD_STATUS 0 NUMERIC_STATUS)
        if(NOT ${NUMERIC_STATUS} EQUAL 0)
          file(REMOVE ${NINJA_ZIP_FILE})
          message(FATAL_ERROR "- Download result: ${DOWNLOAD_STATUS}")
        else()
          message("- Download succeeded")
        endif()    
      endif()
      
      if(EXISTS ${NINJA_ZIP_FILE})
        file(MAKE_DIRECTORY ${NINJA_DIR})
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${NINJA_ZIP_FILE}
                        WORKING_DIRECTORY                  ${NINJA_DIR}
                        RESULT_VARIABLE NINJA_ZIP_FILE_EXTRACT_RESULT)
        if(${NINJA_ZIP_FILE_EXTRACT_RESULT} EQUAL 0)
          file(REMOVE ${NINJA_ZIP_FILE})
        else()
          file(REMOVE ${NINJA_ZIP_FILE})
          file(REMOVE_RECURSE ${NINJA_DIR})
          message(FATAL_ERROR "- Extract error: ${NINJA_ZIP_FILE}")
        endif()
      endif()
    else(NOT EXISTS ${NINJA_DIR})
      message("- Download gcc not required: ${NINJA_DIR}")       
    endif()
    set (CMAKE_MAKE_PROGRAM "${NINJA_DIR}/ninja.exe" PARENT_SCOPE)
    set (CMAKE_MAKE_PROGRAM "${NINJA_DIR}/ninja.exe")
    set(CMAKE_MAKE_PROGRAM   ${CMAKE_MAKE_PROGRAM} CACHE FILEPATH "" FORCE)
    message("- Ninja: ${CMAKE_MAKE_PROGRAM}")
  endif()
  
  set(CMAKE_SYSTEM_NAME       Generic   PARENT_SCOPE)
  set(CMAKE_SYSTEM_PROCESSOR  arm       PARENT_SCOPE)
  
  set(CMAKE_CROSSCOMPILING 1)
  
  # specify the cross compiler
  set(CMAKE_ASM_COMPILER ${GCC_DIR}/bin/arm-none-eabi-as      PARENT_SCOPE)
  set(CMAKE_C_COMPILER   ${GCC_DIR}/bin/arm-none-eabi-gcc.exe PARENT_SCOPE)
  set(CMAKE_CXX_COMPILER ${GCC_DIR}/bin/arm-none-eabi-g++.exe PARENT_SCOPE)
  set(CMAKE_LINKER       ${GCC_DIR}/bin/arm-none-eabi-ld.exe  PARENT_SCOPE)
  
  # search for programs in the build host directories
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER             PARENT_SCOPE)
  # for libraries and headers in the target directories
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY              PARENT_SCOPE)
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY              PARENT_SCOPE)
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY              PARENT_SCOPE)
else()

endif()

set(CC_LINK_TYPE STATIC PARENT_SCOPE)
set(GENERIC)