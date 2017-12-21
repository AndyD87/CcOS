################################################################################
# Compiler Settings
################################################################################
message("- Set Flags for GCC")

# use std::11 as basic !
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=c11  ")#  -static-libgcc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")#  -static-libstdc++")

if(NOT CCOS_LINK_TYPE)
  if(GENERIC)
    set(CCOS_LINK_TYPE STATIC)
  else()
    set(CCOS_LINK_TYPE SHARED)
  endif()    
endif()

if(CCOS_EXECUTABLE_SUFFIX)
  set(CMAKE_EXECUTABLE_SUFFIX ${CCOS_EXECUTABLE_SUFFIX})
endif()

################################################################################
# Enable all Warnings
################################################################################
if(NOT ${CMAKE_CXX_FLAGS} MATCHES "-Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()
if(NOT ${CMAKE_C_FLAGS} MATCHES "-Wall")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()
if(NOT ${CMAKE_CXX_FLAGS} MATCHES "-Wextra")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
endif()
if(NOT ${CMAKE_C_FLAGS} MATCHES "-Wextra")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
endif()

# If required enable Warning As error for each known build type
if(CC_WARNING_AS_ERROR)
  if(NOT ${CMAKE_CXX_FLAGS} MATCHES "-Werror")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif()
  
  if(NOT ${CMAKE_C_FLAGS} MATCHES "-Wall")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  endif()
endif(CC_WARNING_AS_ERROR)

################################################################################
# Set default paths for POSIX Systems
################################################################################
set( PROGRAM_DATA_DIR   "/var/lib" )
set( PROGRAM_CONFIG_DIR "/etc" )
set( PROGRAM_BIN_DIR    "/bin" )
set( PROGRAM_LIB_DIR    "/lib" )

################################################################################
# Try to recognize target architecture
################################################################################
if(WINDOWS)
  # Try to dectect Target Architecture
  include(CheckSymbolExists)
  
  check_symbol_exists("__x86_64__" "" ARCH_X64)
  if(NOT ARCH_X64)
    check_symbol_exists("__i386__" "" ARCH_X86)
    if(NOT ARCH_X86)
      check_symbol_exists("__arm__" "" ARCH_ARM)
    endif()
  endif(NOT ARCH_X64)
  
  if(ARCH_X64)
    set(CCOS_BUILD_ARCH x64)
  elseif(ARCH_X86)
    set(CCOS_BUILD_ARCH x86)
  elseif(ARCH_ARM)
    set(CCOS_BUILD_ARCH arm)
  else()
    if(DEFINED CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS)
      string(REGEX MATCH "__i386__" TESTVAR ${CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS})
      if(TESTVAR)
        set(CCOS_BUILD_ARCH x86)
      else()
        string(REGEX MATCH "__i386__" TESTVAR ${CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS})
        if(TESTVAR)
          set(CCOS_BUILD_ARCH x64)
        endif()
      endif()
    endif()
  endif()
endif()
