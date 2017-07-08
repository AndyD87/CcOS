###################
# Compiler Settings
###################
MESSAGE("Configuration for Windows Platform is called\n")

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

if(NOT CCOS_LINK_TYPE)
    set(CCOS_LINK_TYPE SHARED)
endif()
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

#Set default subsystem to console
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE ")
