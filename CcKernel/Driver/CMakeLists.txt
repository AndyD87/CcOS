########
# Add Source Files
########
file (GLOB IDriver_SOURCES
      "*.cpp"
      "*.h")

set( IDriver_HEADERS )
set( IDriver_SOURCES )
set( IDriver_NAMES_0 )
set( IDriver_NAMES_1 )
set( IDriver_NAMES_2 )
set( IDriver_NAMES_3 )
set( IDriver_INCLUDES )
set( IDriver_DEFINITIONS )


#######
# Setup Driver Makros
#######

macro (add_to_driver_sources)
    foreach (_src ${ARGN})
        CcListAppendOnce(IDriver_SOURCES "${_src}")
    endforeach()
    set (IDriver_SOURCES ${IDriver_SOURCES} PARENT_SCOPE)
endmacro()

macro (add_to_driver_headers)
    foreach (_src ${ARGN})
        CcListAppendOnce(IDriver_HEADERS "${_src}")
    endforeach()
    set (IDriver_HEADERS ${IDriver_HEADERS} PARENT_SCOPE)
endmacro()

macro (add_to_driver_names BootOrder)
    if(${BootOrder} EQUAL 0)
      set(TargetList "IDriver_NAMES_0")
    elseif(${BootOrder} EQUAL 1)
      set(TargetList "IDriver_NAMES_1")
    elseif(${BootOrder} EQUAL 2)
      set(TargetList "IDriver_NAMES_2")
    elseif(${BootOrder} EQUAL 3)
      set(TargetList "IDriver_NAMES_3")
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
        CcListAppendOnce(IDriver_INCLUDES "${_src}")
    endforeach()
    set (IDriver_INCLUDES ${IDriver_INCLUDES} PARENT_SCOPE)
endmacro()

macro (add_to_driver_definitions)
    foreach (_src ${ARGN})
        CcListAppendOnce(IDriver_DEFINITIONS "${_src}")
    endforeach()
    set (IDriver_DEFINITIONS ${IDriver_DEFINITIONS} PARENT_SCOPE)
endmacro()

macro (push_up_driver_vars)
  add_to_driver_sources     ( ${IDriver_SOURCES} )
  add_to_driver_headers     ( ${IDriver_HEADERS} )
  add_to_driver_names       ( 0 ${IDriver_NAMES_0} )
  add_to_driver_names       ( 1 ${IDriver_NAMES_1} )
  add_to_driver_names       ( 2 ${IDriver_NAMES_2} )
  add_to_driver_names       ( 3 ${IDriver_NAMES_3} )
  add_to_driver_includes    ( ${IDriver_INCLUDES} )
  add_to_driver_definitions ( ${IDriver_DEFINITIONS} )
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
set(IDriverLOAD_TEMP    ${CMAKE_CURRENT_BINARY_DIR}/IDriverLoad.cpp.tmp)
set(IDriverLOAD_TARGET  ${CMAKE_CURRENT_BINARY_DIR}/IDriverLoad.cpp)
file(WRITE ${IDriverLOAD_TEMP} "/* Generated File */\n")
file(APPEND ${IDriverLOAD_TEMP} "#include \"Driver/IDriverLoad.h\"\n")
if(IDriver_HEADERS)
  list(REMOVE_DUPLICATES IDriver_HEADERS)
  foreach( headerString ${IDriver_HEADERS} )
    file(APPEND ${IDriverLOAD_TEMP} "#include \"Driver/${headerString}\"\n")
  endforeach()
endif()
file(APPEND ${IDriverLOAD_TEMP} "int IDriverLoad::s_iState(-1);\n")


if(IDriver_NAMES_0)
  list(REMOVE_DUPLICATES IDriver_NAMES_0)
endif()
file(APPEND ${IDriverLOAD_TEMP} "void IDriverLoad::load0()\n")
file(APPEND ${IDriverLOAD_TEMP} "{\n")
file(APPEND ${IDriverLOAD_TEMP} "  // Load Boot Level 0\n")
foreach( nameString ${IDriver_NAMES_0} )
  file(APPEND ${IDriverLOAD_TEMP} "  IDriver* p${nameString} = new ${nameString}();\n")
  file(APPEND ${IDriverLOAD_TEMP} "  p${nameString}->entry();\n")
  file(APPEND ${IDriverLOAD_TEMP} "  m_DriverList.append(p${nameString});\n")
endforeach()
file(APPEND ${IDriverLOAD_TEMP} "}\n")
  

if(IDriver_NAMES_1)
  list(REMOVE_DUPLICATES IDriver_NAMES_1)
endif()
file(APPEND ${IDriverLOAD_TEMP} "void IDriverLoad::load1()\n")
file(APPEND ${IDriverLOAD_TEMP} "{\n")
file(APPEND ${IDriverLOAD_TEMP} "  // Load Boot Level 1\n")
foreach( nameString ${IDriver_NAMES_1} )
  file(APPEND ${IDriverLOAD_TEMP} "  IDriver* p${nameString} = new ${nameString}();\n")
  file(APPEND ${IDriverLOAD_TEMP} "  p${nameString}->entry();\n")
  file(APPEND ${IDriverLOAD_TEMP} "  m_DriverList.append(p${nameString});\n")
endforeach()
file(APPEND ${IDriverLOAD_TEMP} "}\n")

file(APPEND ${IDriverLOAD_TEMP} "  // Load Boot Level 2\n")
if(IDriver_NAMES_2)
  list(REMOVE_DUPLICATES IDriver_NAMES_2)
endif()
file(APPEND ${IDriverLOAD_TEMP} "void IDriverLoad::load2()\n")
file(APPEND ${IDriverLOAD_TEMP} "{\n")
file(APPEND ${IDriverLOAD_TEMP} "  // Load Boot Level 2\n")
foreach( nameString ${IDriver_NAMES_2} )
  file(APPEND ${IDriverLOAD_TEMP} "  IDriver* p${nameString} = new ${nameString}();\n")
  file(APPEND ${IDriverLOAD_TEMP} "  p${nameString}->entry();\n")
  file(APPEND ${IDriverLOAD_TEMP} "  m_DriverList.append(p${nameString});\n")
endforeach()
file(APPEND ${IDriverLOAD_TEMP} "}\n")

if(IDriver_NAMES_3)
  list(REMOVE_DUPLICATES IDriver_NAMES_3)
endif()
file(APPEND ${IDriverLOAD_TEMP} "void IDriverLoad::load3()\n")
file(APPEND ${IDriverLOAD_TEMP} "{\n")
file(APPEND ${IDriverLOAD_TEMP} "  // Load Boot Level 3\n")
foreach( nameString ${IDriver_NAMES_3} )
  file(APPEND ${IDriverLOAD_TEMP} "  IDriver* p${nameString} = new ${nameString}();\n")
  file(APPEND ${IDriverLOAD_TEMP} "  p${nameString}->entry();\n")
  file(APPEND ${IDriverLOAD_TEMP} "  m_DriverList.append(p${nameString});\n")
endforeach()
file(APPEND ${IDriverLOAD_TEMP} "}\n")

CcCopyFile(${IDriverLOAD_TEMP} ${IDriverLOAD_TARGET})

list(APPEND IDriver_SOURCES ${IDriverLOAD_TARGET} )

add_to_kernel_sources(${IDriver_SOURCES})
add_to_kernel_includes(${IDriver_INCLUDES})
add_to_kernel_definitions(${IDriver_DEFINITIONS})