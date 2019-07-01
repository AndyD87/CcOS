if( WIN32 )
  # this one is important
  set( WORKING_DIR "${CC_CACHE_DIR}/Toolchains" )
  set( GCC_DIR      "${WORKING_DIR}/mingw-w64" )
  
  CcDownloadAndExtract( "Mingw-w64" 
                        "${GCC_DIR}"
                        "http://coolcow.de/projects/ThirdParty/Mingw-w64/binaries/6.0.0.0/mingw-w64.7z")
  set(CMAKE_C_COMPILER            "${GCC_DIR}/i686-8.1.0-win32-dwarf-rt_v6-rev0/mingw32/bin/gcc.exe"             CACHE INTERNAL "")
  set(CMAKE_CXX_COMPILER          "${GCC_DIR}/i686-8.1.0-win32-dwarf-rt_v6-rev0/mingw32/bin/g++.exe"             CACHE INTERNAL "")
  set(CMAKE_MAKE_PROGRAM          "${GCC_DIR}/i686-8.1.0-win32-dwarf-rt_v6-rev0/mingw32/bin/mingw32-make.exe"    CACHE INTERNAL "")
  set(ENV{PATH} "${GCC_DIR}/i686-8.1.0-win32-dwarf-rt_v6-rev0/mingw32/bin")
else()
  message(FATAL_ERROR "MinGW only available in Windows")
endif()
