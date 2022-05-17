set( CCPYTHON_VERSION  "3.9.10.0" )
set( CCPYTHON_DIR      "${CC_CACHE_DIR}/Tools/python" )

set(ENV{CONAN_USER_HOME}        "${CC_CACHE_DIR}/conan")
set(ENV{CONAN_USER_HOME_SHORT}  "None")
  
if(NOT EXISTS ${CC_CACHE_DIR}/conan)
  file(MAKE_DIRECTORY ${CC_CACHE_DIR}/conan)
endif()

if(WIN32)
  CcDownloadAndExtract( "conan"
                        "${CCPYTHON_DIR}"
                        "https://coolcow.de/projects/ThirdParty/WinPython/binaries/${CCPYTHON_VERSION}/WinPython.64bit.7z"
                        SHA1 "b99c2f013bd0e52f88ab95acaa8e6c0c8dd14d00"
  )
  
  set(ENV{PATH}                   "${CC_CACHE_DIR}/Tools/python/Scripts;${CC_CACHE_DIR}/Tools/python;$ENV{PATH}")
else()

endif()

find_program(CONAN conan)

if(NOT ${CONAN} STREQUAL "CONAN-NOTFOUND")
  set(CONAN_FOUND TRUE CACHE INTERNAL "Conan was found marker" )
  
  macro(ConanInstall _target)
    message("- Conan install: ${_target}")
    execute_process(
                    COMMAND ${CONAN} install ${_target}
                    OUTPUT_VARIABLE CONAN_OUTPUT   
                    ECHO_OUTPUT_VARIABLE    
                    COMMAND_ECHO STDOUT
                    WORKING_DIRECTORY ${CC_CACHE_DIR}/conan
    )
  endmacro(ConanInstall)
  
  macro(ConanRemoteAdd _name _url)
    execute_process(
                    COMMAND ${CONAN} remote add ${_name} ${_url}
                    OUTPUT_VARIABLE CONAN_OUTPUT   
                    ECHO_OUTPUT_VARIABLE    
                    WORKING_DIRECTORY ${CC_CACHE_DIR}/conan
    )
  endmacro(ConanRemoteAdd)
  
  macro(ConanRemoteUser _name _username _password)
    execute_process(
                    COMMAND ${CONAN} user -r ${_name} ${_username} -p ${_password}
                    OUTPUT_VARIABLE CONAN_OUTPUT   
                    ECHO_OUTPUT_VARIABLE    
                    WORKING_DIRECTORY ${CC_CACHE_DIR}/conan
    )
  endmacro(ConanRemoteUser)
endif()