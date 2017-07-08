###################
# Compiler Settings
###################
MESSAGE("Set Flags for GCC")

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
  
set( PROGRAM_DATA_DIR   "/var/CcOS" )
set( PROGRAM_CONFIG_DIR "/etc/CcOS" )
set( PROGRAM_BIN_DIR    "/bin" )
set( PROGRAM_LIB_DIR    "/lib" )
