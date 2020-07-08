if( WIN32 )
  # this one is important
  set( WORKING_DIR "${CC_CACHE_DIR}/Toolchains" )
  if(${MINGW_VERSION} VERSION_EQUAL "4.8.2.0")
    # Make sure Version string has correct formate for URL
    set( MINGW_VERSION      "4.8.2.0" )
    set( MINGW_32BIT_PATH   "i686-4.8.2-release-win32-dwarf-rt_v3-rev2" )
    set( MINGW_64BIT_PATH   "x86_64-4.8.2-release-win32-seh-rt_v3-rev4" )
  else()
    # Take latest if version not set or unknown
    set( MINGW_VERSION      "6.0.0.0" )
    set( MINGW_32BIT_PATH   "i686-8.1.0-win32-dwarf-rt_v6-rev0" )
    set( MINGW_64BIT_PATH   "x86_64-8.1.0-win32-seh-rt_v6-rev0" )
  endif()
  set( GCC_DIR            "${WORKING_DIR}/${MINGW_VERSION}/mingw-w64" )

  CcDownloadAndExtract( "Mingw-w64"
                        "${GCC_DIR}"
                        "http://coolcow.de/projects/ThirdParty/Mingw-w64/binaries/${MINGW_VERSION}/mingw-w64.7z"
  )
  # Select 32bit or 64bit toolchain
  if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "86" )
    set(CMAKE_C_COMPILER            "${GCC_DIR}/${MINGW_32BIT_PATH}/mingw32/bin/gcc.exe"             )
    set(CMAKE_CXX_COMPILER          "${GCC_DIR}/${MINGW_32BIT_PATH}/mingw32/bin/g++.exe"             )
    set(CMAKE_MAKE_PROGRAM          "${GCC_DIR}/${MINGW_32BIT_PATH}/mingw32/bin/mingw32-make.exe"    )
    set(ENV{PATH} "${GCC_DIR}/${MINGW_32BIT_PATH}/mingw32/bin")
  elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "32" )
    set(CMAKE_C_COMPILER            "${GCC_DIR}/${MINGW_32BIT_PATH}/mingw32/bin/gcc.exe"             )
    set(CMAKE_CXX_COMPILER          "${GCC_DIR}/${MINGW_32BIT_PATH}/mingw32/bin/g++.exe"             )
    set(CMAKE_MAKE_PROGRAM          "${GCC_DIR}/${MINGW_32BIT_PATH}/mingw32/bin/mingw32-make.exe"    )
    set(ENV{PATH} "${GCC_DIR}/${MINGW_32BIT_PATH}/mingw32/bin")
  else()
    set(CMAKE_C_COMPILER            "${GCC_DIR}/${MINGW_64BIT_PATH}/mingw64/bin/gcc.exe"             )
    set(CMAKE_CXX_COMPILER          "${GCC_DIR}/${MINGW_64BIT_PATH}/mingw64/bin/g++.exe"             )
    set(CMAKE_MAKE_PROGRAM          "${GCC_DIR}/${MINGW_64BIT_PATH}/mingw64/bin/mingw32-make.exe"    )
    set(ENV{PATH} "${GCC_DIR}/${MINGW_64BIT_PATH}/mingw64/bin")
  endif()
else()
  set(CMAKE_SYSTEM_NAME "Windows" )
endif()
