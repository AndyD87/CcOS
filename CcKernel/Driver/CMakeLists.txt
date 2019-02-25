########
# Add Source Files
########
file (GLOB CCDRIVER_SOURCES
      "*.cpp"
      "*.h")

set( CCDRIVER_HEADERS )
set( CCDRIVER_SOURCES )
set( CCDRIVER_NAMES_0 )
set( CCDRIVER_NAMES_1 )
set( CCDRIVER_NAMES_2 )
set( CCDRIVER_NAMES_3 )
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

macro (add_to_driver_names BootOrder)
    if(${BootOrder} EQUAL 0)
      set(TargetList "CCDRIVER_NAMES_0")
    elseif(${BootOrder} EQUAL 1)
      set(TargetList "CCDRIVER_NAMES_1")
    elseif(${BootOrder} EQUAL 2)
      set(TargetList "CCDRIVER_NAMES_2")
    elseif(${BootOrder} EQUAL 3)
      set(TargetList "CCDRIVER_NAMES_3")
    else()
      message(FATAL_ERROR "Wrong boot order: ${BootOrder}")
    endif()
    
    foreach (_src ${ARGN})
      CcListAppendOnce(${TargetList} "${_src}")
    endforeach()
    set (${TargetList} ${${TargetList}} PARENT_SCOPE)
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
  add_to_driver_names       ( 0 ${CCDRIVER_NAMES_0} )
  add_to_driver_names       ( 1 ${CCDRIVER_NAMES_1} )
  add_to_driver_names       ( 2 ${CCDRIVER_NAMES_2} )
  add_to_driver_names       ( 3 ${CCDRIVER_NAMES_3} )
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

if( CCOS_DRIVER_BOARD)
  add_subdirectory(Board)
endif ()

################################################################################
# Write Data to Driverload.cpp
################################################################################
set(CCDRIVERLOAD_TEMP    ${CMAKE_CURRENT_BINARY_DIR}/CcDriverLoad.cpp.tmp)
set(CCDRIVERLOAD_TARGET  ${CMAKE_CURRENT_BINARY_DIR}/CcDriverLoad.cpp)
file(WRITE ${CCDRIVERLOAD_TEMP} "/* Generated File */\n")
file(APPEND ${CCDRIVERLOAD_TEMP} "#include \"Driver/CcDriverLoad.h\"\n")
if(CCDRIVER_HEADERS)
  list(REMOVE_DUPLICATES CCDRIVER_HEADERS)
  foreach( headerString ${CCDRIVER_HEADERS} )
    file(APPEND ${CCDRIVERLOAD_TEMP} "#include \"Driver/${headerString}\"\n")
  endforeach()
endif()
file(APPEND ${CCDRIVERLOAD_TEMP} "// avoid warnings for empty sourcefiles by adding an void function\n")
file(APPEND ${CCDRIVERLOAD_TEMP} "int CcDriverLoadNullFunction()\n")
file(APPEND ${CCDRIVERLOAD_TEMP} "{\n")
file(APPEND ${CCDRIVERLOAD_TEMP} "  return 0;\n")
file(APPEND ${CCDRIVERLOAD_TEMP} "}\n")
file(APPEND ${CCDRIVERLOAD_TEMP} "void CcDriverLoad::init(void)\n")
file(APPEND ${CCDRIVERLOAD_TEMP} "{\n")
file(APPEND ${CCDRIVERLOAD_TEMP} "  // Load Boot Level 0\n")
if(CCDRIVER_NAMES_0)
  list(REMOVE_DUPLICATES CCDRIVER_NAMES_0)
  foreach( nameString ${CCDRIVER_NAMES_0} )
    file(APPEND ${CCDRIVERLOAD_TEMP} "  CcDriver* p${nameString} = new ${nameString}();\n")
    file(APPEND ${CCDRIVERLOAD_TEMP} "  p${nameString}->entry();\n")
  endforeach()
endif()
file(APPEND ${CCDRIVERLOAD_TEMP} "  // Load Boot Level 1\n")
if(CCDRIVER_NAMES_1)
  list(REMOVE_DUPLICATES CCDRIVER_NAMES_1)
  foreach( nameString ${CCDRIVER_NAMES_1} )
    file(APPEND ${CCDRIVERLOAD_TEMP} "  CcDriver* p${nameString} = new ${nameString}();\n")
    file(APPEND ${CCDRIVERLOAD_TEMP} "  p${nameString}->entry();\n")
  endforeach()
endif()
file(APPEND ${CCDRIVERLOAD_TEMP} "  // Load Boot Level 2\n")
if(CCDRIVER_NAMES_2)
  list(REMOVE_DUPLICATES CCDRIVER_NAMES_2)
  foreach( nameString ${CCDRIVER_NAMES_2} )
    file(APPEND ${CCDRIVERLOAD_TEMP} "  CcDriver* p${nameString} = new ${nameString}();\n")
    file(APPEND ${CCDRIVERLOAD_TEMP} "  p${nameString}->entry();\n")
  endforeach()
endif()
file(APPEND ${CCDRIVERLOAD_TEMP} "  // Load Boot Level 3\n")
if(CCDRIVER_NAMES_3)
  list(REMOVE_DUPLICATES CCDRIVER_NAMES_3)
  foreach( nameString ${CCDRIVER_NAMES_3} )
    file(APPEND ${CCDRIVERLOAD_TEMP} "  CcDriver* p${nameString} = new ${nameString}();\n")
    file(APPEND ${CCDRIVERLOAD_TEMP} "  p${nameString}->entry();\n")
  endforeach()
endif()
file(APPEND ${CCDRIVERLOAD_TEMP} "}\n")

CcCopyFile(${CCDRIVERLOAD_TEMP} ${CCDRIVERLOAD_TARGET})



list(APPEND CCDRIVER_SOURCES ${CCDRIVERLOAD_TARGET} )

add_to_kernel_sources(${CCDRIVER_SOURCES})
add_to_kernel_includes(${CCDRIVER_INCLUDES})
add_to_kernel_definitions(${CCDRIVER_DEFINITIONS})