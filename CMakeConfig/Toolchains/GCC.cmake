################################################################################
# Compiler Settings
################################################################################
message("- Set Flags for GCC")

if(WINDOWS)
  add_definitions(-DWINDOWS)
  set(LINKER_FLAGS "-Wl,-Bstatic,--whole-archive -static-libgcc -static-libstdc++ -Wl,-allow-multiple-definition")
  CcAppendSharedLinkerFlags(${LINKER_FLAGS})
  CcAppendExeLinkerFlags(${LINKER_FLAGS})
  set(MINGW  TRUE)
endif()

# use std::11 as basic !
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=c11  ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker -Map=output.map")

if(NOT CC_LINK_TYPE)
  if(GENERIC)
    set(CC_LINK_TYPE STATIC)
  else()
    set(CC_LINK_TYPE SHARED)
  endif()
endif()

if(CC_EXECUTABLE_SUFFIX)
  message("- Suffix: ${CC_EXECUTABLE_SUFFIX}")
  set(CMAKE_EXECUTABLE_SUFFIX ${CC_EXECUTABLE_SUFFIX})
endif()

# Set default build type to release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

################################################################################
# Enable all Warnings
################################################################################
CcAppendCompilerFlags("-Wall")
CcAppendCompilerFlags("-Wextra")

# If required enable Warning As error for each known build type
if(CC_WARNING_AS_ERROR)
  CcAppendCxxCompilerFlags("-Werror")
endif(CC_WARNING_AS_ERROR)

################################################################################
# Set -fPIC value for all libraries, also static libraries, so they can be
# linked from shared too.
################################################################################
if(LINUX OR WINDOWS)
	set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
endif()

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
# Try to dectect Target Architecture
include(CheckSymbolExists)

check_symbol_exists("__x86_64__" "" ARCH_X64)
if(NOT ARCH_X64)
  check_symbol_exists("__MINGW64__" "" ARCH_X64)
  if(NOT ARCH_X64)
    check_symbol_exists("__i386__" "" ARCH_X86)
    if(NOT ARCH_X86)
      check_symbol_exists("__arm__" "" ARCH_ARM)
    endif()
  endif()
endif(NOT ARCH_X64)

if(ARCH_X64)
  set(CC_BUILD_ARCH x64)
elseif(ARCH_X86)
  set(CC_BUILD_ARCH x86)
elseif(ARCH_ARM)
  set(CC_BUILD_ARCH arm)
else()
  if(DEFINED CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS)
    string(REGEX MATCH "__i386__" TESTVAR ${CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS})
    if(TESTVAR)
      set(CC_BUILD_ARCH x86)
    else()
      string(REGEX MATCH "__i386__" TESTVAR ${CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS})
      if(TESTVAR)
        set(CC_BUILD_ARCH x64)
      else()
        set(CC_BUILD_ARCH unknown)
      endif()
    endif()
  else()
    set(CC_BUILD_ARCH unknown)
  endif()
endif()

if(${CC_BUILD_ARCH} STREQUAL unknown)
  # Try to get info from compiler
  execute_process(COMMAND "${CMAKE_C_COMPILER}" "-v"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  RESULT_VARIABLE COMPILER_RESULT
                  OUTPUT_VARIABLE "COMPILER_INFO"
                  ERROR_VARIABLE  "COMPILER_INFO")
  if(${COMPILER_INFO} MATCHES "xtensa")
    message("-- Architecture detected: xtensa")
    set(CC_BUILD_ARCH xtensa)
  elseif(${COMPILER_INFO} MATCHES "mingw64")
    message("-- Architecture detected: mingw x64")
    set(CC_BUILD_ARCH x64)
  elseif(${COMPILER_INFO} MATCHES "mingw32")
    message("-- Architecture detected: mingw x86")
    set(CC_BUILD_ARCH x86)
  else()
    message("-- Architecture not found in extra generator, set unknown")
  endif()
endif()

if(${CC_BUILD_ARCH} STREQUAL x64)
  add_definitions(-DCC_BUILD_ARCH=${CC_BUILD_ARCH_X64})
elseif(${CC_BUILD_ARCH} STREQUAL x86)
  add_definitions(-DCC_BUILD_ARCH=${CC_BUILD_ARCH_X86})
elseif(${CC_BUILD_ARCH} STREQUAL arm)
  add_definitions(-DCC_BUILD_ARCH=${CC_BUILD_ARCH_ARM})
elseif(${CC_BUILD_ARCH} STREQUAL arm64)
  add_definitions(-DCC_BUILD_ARCH=${CC_BUILD_ARCH_ARM64})
elseif(${CC_BUILD_ARCH} STREQUAL xtensa)
  add_definitions(-DCC_BUILD_ARCH=${CC_BUILD_ARCH_XTENSA})
else()
  add_definitions(-DCC_BUILD_ARCH=-1)
endif()
################################################################################
# Change output targets for gcc
################################################################################
if(NOT CC_OUTPUT_PREFIX)
  string( TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE_LOWER )
  set(CC_OUTPUT_PREFIX "${CC_BUILD_ARCH}_${BUILD_TYPE_LOWER}")

  # Set runtime output dir to root/output if no other location was defined
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CC_OUTPUT_DIR}/${CC_OUTPUT_PREFIX}/bin")
  endif()

  # Set library output dir to root/output if no other location was defined
  if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CC_OUTPUT_DIR}/${CC_OUTPUT_PREFIX}/lib")
  endif()

  # Set archive output dir to root/output if no other location was defined
  if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CC_OUTPUT_DIR}/${CC_OUTPUT_PREFIX}/lib/static")
  else()
    message(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  endif()

  foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOLOWER ${OUTPUTCONFIG} OUTPUTCONFIGLOWER )
    set(CC_OUTPUT_PREFIX ${OUTPUTCONFIGLOWER})

    # Set runtime output dir to root/output if no other location was defined
    if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG})
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CC_OUTPUT_DIR}/${CC_OUTPUT_PREFIX}/bin")
    endif()

    # Set library output dir to root/output if no other location was defined
    if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG})
      set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CC_OUTPUT_DIR}/${CC_OUTPUT_PREFIX}/lib")
    endif()

    # Set archive output dir to root/output if no other location was defined
    if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG})
      set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CC_OUTPUT_DIR}/${CC_OUTPUT_PREFIX}/lib/static")
    endif()
  endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
endif(NOT CC_OUTPUT_PREFIX)
