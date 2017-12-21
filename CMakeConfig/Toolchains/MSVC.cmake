################################################################################
# Compiler Settings for MSVC
################################################################################
cmake_minimum_required (VERSION 3.2)
message("- Configuration for Windows Platform is called\n")

set(CCOS_LINK_TYPE_RUNTIME STATIC)
set(CCOS_BUILD_ARCH Unknown)

################################################################################
# Setup default Variables if not previously set
################################################################################
if(NOT CCOS_LINK_TYPE)
  set(CCOS_LINK_TYPE SHARED)
elseif("${CCOS_LINK_TYPE}" STREQUAL "STATIC")
  add_definitions(-DCC_STATIC)
endif()

if(NOT CCOS_LINK_TYPE_RUNTIME)
  set(CCOS_LINK_TYPE_RUNTIME STATIC)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

################################################################################
# Set common compiler settings: 
#  - default WarningLevel 4
#  - static runtime if required
################################################################################
message("- Compiler: ${CMAKE_C_COMPILER_ID}")
set ( CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS_MINSIZEREL
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS_MINSIZEREL
    )

# Enable Warning Level 4
foreach(CompilerFlag ${CompilerFlags})
  if(${CompilerFlag} MATCHES "/W[0-4]")
    string(REPLACE "/W0" "/W4" ${CompilerFlag} "${${CompilerFlag}}")
    string(REPLACE "/W1" "/W4" ${CompilerFlag} "${${CompilerFlag}}")
    string(REPLACE "/W2" "/W4" ${CompilerFlag} "${${CompilerFlag}}")
    string(REPLACE "/W3" "/W4" ${CompilerFlag} "${${CompilerFlag}}")
  else()
    set(${CompilerFlag} "${${CompilerFlag}} /W4")
  endif()
  if("${CCOS_LINK_TYPE_RUNTIME}" STREQUAL "STATIC")
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  endif()
endforeach()

# Enable Warning As Error if requested
if(CC_WARNING_AS_ERROR)
  # Enable Warning As error for each known build type
  foreach(CompilerFlag ${CompilerFlags})
    if(${CompilerFlag} MATCHES "/WX")
      # do not set /WX twice
    else()
      set(${CompilerFlag} "${${CompilerFlag}} /WX")
    endif()
  endforeach()
endif(CC_WARNING_AS_ERROR)

################################################################################
# Set common linker flags: 
#  - subsystem console 
################################################################################
set ( CompilerFlags
        CMAKE_EXE_LINKER_FLAGS
        CMAKE_EXE_LINKER_FLAGS_DEBUG
        CMAKE_EXE_LINKER_FLAGS_RELEASE
        CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
        CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
    )
foreach(CompilerFlag ${CompilerFlags})
  set(${CompilerFlag} " ${${CompilerFlag}} /SUBSYSTEM:CONSOLE")
endforeach()

################################################################################
# Change output targets for Windows
################################################################################
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

# Try to get target platform by MSVC_C_ARCHITECTURE_ID
if(MSVC_C_ARCHITECTURE_ID)
  if("${MSVC_C_ARCHITECTURE_ID}" STREQUAL "X86")
    set(CCOS_BUILD_ARCH x86)
  elseif("${MSVC_C_ARCHITECTURE_ID}" STREQUAL "x64")
    set(CCOS_BUILD_ARCH x64)
  else()
    message(FATAL_ERROR "Unknown architecture in ${MSVC_C_ARCHITECTURE_ID}")
  endif()
# Try to get target platform by CMAKE_GENERATOR_PLATFORM
elseif(CMAKE_GENERATOR_PLATFORM)
  if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "win32")
    set(CCOS_BUILD_ARCH x86)
  elseif("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    set(CCOS_BUILD_ARCH x64)
  else()
    message(FATAL_ERROR "Unknown architecture in ${CMAKE_GENERATOR_PLATFORM}")
  endif()
else()
  include(CheckSymbolExists)
  check_symbol_exists("_M_AMD64" "" ARCH_X64)
  if(NOT ARCH_X64)
    check_symbol_exists("_M_IX86" "" ARCH_X86)
  endif(NOT ARCH_X64)
  
  if(ARCH_X64)
    set(CCOS_BUILD_ARCH x64)
  elseif(ARCH_X86)
    set(CCOS_BUILD_ARCH x86)
  else()
    message(FATAL_ERROR "Unknown architecture")
  endif()
endif()