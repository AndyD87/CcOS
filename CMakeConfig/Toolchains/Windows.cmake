###################
# Compiler Settings
###################
MESSAGE("Configuration for Windows Platform is called\n")

# Check if an Toolchain is set
if(DEFINED TARGET_TOOLCHAIN)
  if( ${TARGET_TOOLCHAIN} STREQUAL "arm-none")
    set(CMAKE_SYSTEM_NAME            Generic)
    INCLUDE(CMakeForceCompiler)
    CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
    CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)
    SET(CMAKE_FIND_ROOT_PATH  C:/gcc-arm-none-eabi/4.9/ )
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
  endif()
endif()
if(DEFINED MSVC)
  MESSAGE("Compiler: ${CMAKE_C_COMPILER_ID}")
  set ( CompilerFlags
          CMAKE_CXX_FLAGS
          CMAKE_CXX_FLAGS_DEBUG
          CMAKE_CXX_FLAGS_RELEASE
          CMAKE_C_FLAGS
          CMAKE_C_FLAGS_DEBUG
          CMAKE_C_FLAGS_RELEASE
      )
  foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  endforeach()
  
  # Enable Warning Level 4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  endif()
  set(LINK_TYPE SHARED)
  
  #Set default subsystem to console
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE ")
else()
  MESSAGE("Compiler: Unknown, using gcc settings")
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=c11  ")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
