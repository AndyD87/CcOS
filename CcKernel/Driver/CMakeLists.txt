########
# Add Source Files
########
file (GLOB CCDRIVER_SOURCES
      "src/*.cpp"
      "*.h")

set( CCDRIVER_HEADERS )
set( CCDRIVER_NAMES )


#######
# Setup Driver Makros
#######

macro (add_to_driver_sources)
    foreach (_src ${ARGN})
        list (APPEND CCDRIVER_SOURCES "${_src}")
    endforeach()
    set (CCDRIVER_SOURCES ${CCDRIVER_SOURCES} PARENT_SCOPE)
endmacro()

macro (add_to_driver_headers)
    foreach (_src ${ARGN})
        list (APPEND CCDRIVER_HEADERS "${_src}")
    endforeach()
    set (CCDRIVER_HEADERS ${CCDRIVER_HEADERS} PARENT_SCOPE)
endmacro()

macro (add_to_driver_names)
    foreach (_src ${ARGN})
        list (APPEND CCDRIVER_NAMES "${_src}")
    endforeach()
    set (CCDRIVER_NAMES ${CCDRIVER_NAMES} PARENT_SCOPE)
endmacro()

macro (push_up_driver_vars)
  add_to_driver_sources     ( ${CCDRIVER_SOURCES} )
  add_to_driver_headers     ( ${CCDRIVER_HEADERS} )
  add_to_driver_names       ( ${CCDRIVER_NAMES} )
endmacro()

################################################################################
# Choose Driver depending on OS
################################################################################
if( CCOS_DRIVER_CAMERA)
  add_subdirectory(Camera)
endif()

if( CCOS_DRIVER_CPU)
  add_subdirectory(CPU)
endif ()

################################################################################
# Write Data to Driverload.cpp
################################################################################
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/src/CcDriverLoad.cpp "/* Generated File */\n")
file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/CcDriverLoad.cpp "#include \"Driver/CcDriverLoad.h\"\n")
if(CCDRIVER_HEADERS)
  list(REMOVE_DUPLICATES CCDRIVER_HEADERS)
  foreach( headerString ${CCDRIVER_HEADERS} )
    file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/CcDriverLoad.cpp "#include \"Driver/${headerString}\"\n")
  endforeach()
endif()
file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/CcDriverLoad.cpp "void CcDriverLoad::init(void)\n")
file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/CcDriverLoad.cpp "{\n")
if(CCDRIVER_NAMES)
  list(REMOVE_DUPLICATES CCDRIVER_NAMES)
  foreach( nameString ${CCDRIVER_NAMES} )
    file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/CcDriverLoad.cpp "CcDriver* p${nameString} = new ${nameString}();\n")
    file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/CcDriverLoad.cpp "p${nameString}->entry();\n")
  endforeach()
endif()
file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/CcDriverLoad.cpp "}\n")
list(APPEND CCDRIVER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/CcDriverLoad.cpp )

add_to_kernel_sources(${CCDRIVER_SOURCES})
