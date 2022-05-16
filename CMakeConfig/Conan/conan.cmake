
set( CMAKE_SYSTEM_NAME  Linux )
set( CCPYTHON_VERSION  "3.9.10.0" )
set( CCPYTHON_DIR      "${CC_CACHE_DIR}/Tools/python" )

CcDownloadAndExtract( "conan"
                      "${CCPYTHON_DIR}"
                      "https://coolcow.de/projects/ThirdParty/WinPython/binaries/${CCPYTHON_VERSION}/WinPython.64bit.7z"
                      SHA1 "b99c2f013bd0e52f88ab95acaa8e6c0c8dd14d00"
)

set(ENV{PATH}                   "${CC_CACHE_DIR}/Tools/python/Scripts;${CC_CACHE_DIR}/Tools/python;$ENV{PATH}")
set(ENV{CONAN_USER_HOME}        "${CC_CACHE_DIR}/conan")
set(ENV{CONAN_USER_HOME_SHORT}  "None")

macro(ConanInstall _target)
  execute_process(
                  COMMAND cmd /C conan install ${_target}
                  WORKING_DIRECTORY ${CC_CACHE_DIR}/conan
  )
endmacro(ConanInstall)