if(WIN32)
  set( WORKING_DIR "${CC_CACHE_DIR}/Toolchains" )
  set( MAKE_DIR      "${WORKING_DIR}/Cygwin" )
  set( MAKE_BINARY   "${WORKING_DIR}/Cygwin/bin/make")
  
  CcDownloadAndExtract( "Cygwin/make"
                        "${MAKE_DIR}"
                        "https://coolcow.de/projects/ThirdParty/CygwinPackage/binaries/1.0.0.0/cygwin.32bit.7z")

  set(CMAKE_MAKE_PROGRAM "${MAKE_BINARY}" PARENT_SCOPE)
  set(CMAKE_MAKE_PROGRAM "${MAKE_BINARY}")
  set(CMAKE_MAKE_PROGRAM "${MAKE_BINARY}" CACHE FILEPATH "" FORCE)
  message("- make: ${CMAKE_MAKE_PROGRAM}")
endif()