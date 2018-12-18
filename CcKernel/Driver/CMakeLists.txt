########
# Add Source Files
########
file (GLOB CCDRIVER_SOURCES
      "src/*.cpp"
      "*.h")

set( CCDRIVER_HEADERS )
set( CCDRIVER_SOURCES )
set( CCDRIVER_NAMES )
set( CCDRIVER_INCLUDES )
set( CCDRIVER_DEFINITIONS )


#######
# Setup Driver Makros
#######

macro (add_to_driver_sources)
    foreach (_src ${ARGN})
        CcListAppendOnce(CCDRIVER_SOURCES "${_src}")
    endforeach()
    set (CCDRIVER_SOURCES ${CCDRIVER_SOURCES} PARENT_SCOPE)
endmacro()

macro (add_to_driver_headers)
    foreach (_src ${ARGN})
        CcListAppendOnce(CCDRIVER_HEADERS "${_src}")
    endforeach()
    set (CCDRIVER_HEADERS ${CCDRIVER_HEADERS} PARENT_SCOPE)
endmacro()

macro (add_to_driver_names)
    foreach (_src ${ARGN})
        CcListAppendOnce(CCDRIVER_NAMES "${_src}")
    endforeach()
    set (CCDRIVER_NAMES ${CCDRIVER_NAMES} PARENT_SCOPE)
endmacro()

macro (add_to_driver_includes)
    foreach (_src ${ARGN})
        CcListAppendOnce(CCDRIVER_INCLUDES "${_src}")
    endforeach()
    set (CCDRIVER_INCLUDES ${CCDRIVER_INCLUDES} PARENT_SCOPE)
endmacro()

macro (add_to_driver_definitions)
    foreach (_src ${ARGN})
        CcListAppendOnce(CCDRIVER_DEFINITIONS "${_src}")
    endforeach()
    set (CCDRIVER_DEFINITIONS ${CCDRIVER_DEFINITIONS} PARENT_SCOPE)
endmacro()

macro (push_up_driver_vars)
  add_to_driver_sources     ( ${CCDRIVER_SOURCES} )
  add_to_driver_headers     ( ${CCDRIVER_HEADERS} )
  add_to_driver_names       ( ${CCDRIVER_NAMES} )
  add_to_driver_includes    ( ${CCDRIVER_INCLUDES} )
  add_to_driver_definitions ( ${CCDRIVER_DEFINITIONS} )
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
set(CCDRIVERLOAD_TEMP    ${CMAKE_CURRENT_SOURCE_DIR}/src/CcDriverLoad.cpp.tmp)
set(CCDRIVERLOAD_TARGET  ${CMAKE_CURRENT_SOURCE_DIR}/src/CcDriverLoad.cpp)
file(WRITE ${CCDRIVERLOAD_TEMP} "/* Generated File */\n")
file(APPEND ${CCDRIVERLOAD_TEMP} "#include \"Driver/CcDriverLoad.h\"\n")
if(CCDRIVER_HEADERS)
  list(REMOVE_DUPLICATES CCDRIVER_HEADERS)
  foreach( headerString ${CCDRIVER_HEADERS} )
    file(APPEND ${CCDRIVERLOAD_TEMP} "#include \"Driver/${headerString}\"\n")
    file(APPEND ${CCDRIVERLOAD_TEMP} "// avoid warnings for empty sourcefiles by adding an void function\n")
    file(APPEND ${CCDRIVERLOAD_TEMP} "int CcDriverLoadNullFunction()\n")
    file(APPEND ${CCDRIVERLOAD_TEMP} "{\n")
    file(APPEND ${CCDRIVERLOAD_TEMP} "  return 0;\n")
    file(APPEND ${CCDRIVERLOAD_TEMP} "}\n")
  endforeach()
endif()
file(APPEND ${CCDRIVERLOAD_TEMP} "void CcDriverLoad::init(void)\n")
file(APPEND ${CCDRIVERLOAD_TEMP} "{\n")
if(CCDRIVER_NAMES)
  list(REMOVE_DUPLICATES CCDRIVER_NAMES)
  foreach( nameString ${CCDRIVER_NAMES} )
    file(APPEND ${CCDRIVERLOAD_TEMP} "CcDriver* p${nameString} = new ${nameString}();\n")
    file(APPEND ${CCDRIVERLOAD_TEMP} "p${nameString}->entry();\n")
  endforeach()
endif()
file(APPEND ${CCDRIVERLOAD_TEMP} "}\n")

CcCopyFile(${CCDRIVERLOAD_TEMP} ${CCDRIVERLOAD_TARGET})



list(APPEND CCDRIVER_SOURCES ${CCDRIVERLOAD_TARGET} )

add_to_kernel_sources(${CCDRIVER_SOURCES})
add_to_kernel_includes(${CCDRIVER_INCLUDES})
add_to_kernel_definitions(${CCDRIVER_DEFINITIONS})