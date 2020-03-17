################################################################################
# Compiler Settings for MSVC
################################################################################
cmake_minimum_required (VERSION 3.2)
message("- Configuration for Windows Platform is called")

set(CC_LINK_TYPE_RUNTIME STATIC)
set(CC_BUILD_ARCH Unknown)

################################################################################
# Check for WDK
################################################################################
if(WINDOWS_KERNEL)
  find_package(WDK)
  set(CC_LINK_TYPE STATIC)
endif()

################################################################################
# Setup default Variables if not previously set
################################################################################
if(NOT CC_LINK_TYPE)
  set(CC_LINK_TYPE SHARED)
elseif("${CC_LINK_TYPE}" STREQUAL "STATIC")
  add_definitions(-DCC_STATIC)
endif()

if(NOT CC_LINK_TYPE_RUNTIME)
  set(CC_LINK_TYPE_RUNTIME STATIC)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

################################################################################
# Set common compiler settings: 
#  - default WarningLevel 4
#  - static runtime if required
#  - set /INCREMENTAL:NO for debug 
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
  # always set runtime option
  if(${CompilerFlag} MATCHES "/MD")
    if("${CC_LINK_TYPE_RUNTIME}" STREQUAL "STATIC")
      string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endif()
  elseif(${CompilerFlag} MATCHES "/MT")
    # Do nothing
  else()
    if("${CompilerFlag} " MATCHES "DEBUG ")
      set(${CompilerFlag} "${${CompilerFlag}} /MTd")
    else()
      set(${CompilerFlag} "${${CompilerFlag}} /MT")
    endif()
  endif()
  set(${CompilerFlag} "${${CompilerFlag}} /D_UNICODE /DUNICODE")
endforeach()

if("${CC_LINK_TYPE_RUNTIME}" STREQUAL "STATIC")
  foreach(CompilerFlag ${CompilerFlags})
    set(${CompilerFlag} "${${CompilerFlag}} /EHsc")
  endforeach()
endif()

# Enable Warning As Error if requested
if(CC_WARNING_AS_ERROR)
  # Enable Warning As error for each known build type
  foreach(CompilerFlag ${CompilerFlags})
    CcAppendStringNotTwice(${CompilerFlag} " /WX" )
  endforeach()
endif(CC_WARNING_AS_ERROR)

################################################################################
# Set exe linker flags: 
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

CcRemoveString(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/INCREMENTAL:YES")
CcRemoveString(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/INCREMENTAL:NO")
CcRemoveString(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/INCREMENTAL")
CcAppendStringNotTwice(CMAKE_SHARED_LINKER_FLAGS_DEBUG " /INCREMENTAL:NO")

CcRemoveString(CMAKE_EXE_LINKER_FLAGS_DEBUG "/INCREMENTAL:YES")
CcRemoveString(CMAKE_EXE_LINKER_FLAGS_DEBUG "/INCREMENTAL:NO")
CcRemoveString(CMAKE_EXE_LINKER_FLAGS_DEBUG "/INCREMENTAL")
CcAppendStringNotTwice(CMAKE_EXE_LINKER_FLAGS_DEBUG " /INCREMENTAL:NO")


################################################################################
# Get Target Architecture for CC_BUILD_ARCH
################################################################################
# Try to get target platform by MSVC_C_ARCHITECTURE_ID
if(MSVC_C_ARCHITECTURE_ID)
  if("${MSVC_C_ARCHITECTURE_ID}" STREQUAL "X86")
    set(CC_BUILD_ARCH x86)
  elseif("${MSVC_C_ARCHITECTURE_ID}" STREQUAL "x64")
    set(CC_BUILD_ARCH x64)
  else()
    message(FATAL_ERROR "Unknown architecture in ${MSVC_C_ARCHITECTURE_ID}")
  endif()
# Try to get target platform by CMAKE_GENERATOR_PLATFORM
elseif(CMAKE_GENERATOR_PLATFORM)
  if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "win32")
    set(CC_BUILD_ARCH x86)
  elseif("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    set(CC_BUILD_ARCH x64)
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
    set(CC_BUILD_ARCH x64)
  elseif(ARCH_X86)
    set(CC_BUILD_ARCH x86)
  else()
    message(FATAL_ERROR "Unknown architecture")
  endif()
endif()

if(NOT CC_OUTPUT_RELEASE)
  CcVisualStudioPostFix(CC_OUTPUT_RELEASE "RELEASE" "${CC_LINK_TYPE}" "${CC_LINK_TYPE_RUNTIME}")
  set(CC_OUTPUT_RELEASE "${CC_OUTPUT_RELEASE}/")
endif(NOT CC_OUTPUT_RELEASE)

if(NOT CC_OUTPUT_DEBUG)
  set(CC_OUTPUT_DEBUG "${CC_OUTPUT_DEBUG}/")
endif(NOT CC_OUTPUT_DEBUG)

################################################################################
# Change output targets for Windows
################################################################################

if(NOT CC_OUTPUT_PREFIX)
  CcVisualStudioPostFix(CC_OUTPUT_PREFIX "${CMAKE_BUILD_TYPE}" "${CC_LINK_TYPE}" "${CC_LINK_TYPE_RUNTIME}")
  
  # Set runtime output dir to root/output if no other location was defined
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CC_OUTPUT_DIR}/${CC_OUTPUT_PREFIX}/default/bin")
  endif()
  
  # Set library output dir to root/output if no other location was defined
  if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CC_OUTPUT_DIR}/${CC_OUTPUT_PREFIX}/$default/lib")
  endif()
  
  # Set archive output dir to root/output if no other location was defined
  if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CC_OUTPUT_DIR}/${CC_OUTPUT_PREFIX}/default/lib/static")
  else()
    message(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  endif()
  
  foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    CcVisualStudioPostFix(CC_OUTPUT_PREFIX "${OUTPUTCONFIG}" "${CC_LINK_TYPE}" "${CC_LINK_TYPE_RUNTIME}")
    
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
