MACRO (CcSetInstall ProjectName )
  set_property( TARGET ${ProjectName} APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}>
              )

  INSTALL( TARGETS  ${ProjectName}
           EXPORT  "${ProjectName}Config"
           RUNTIME DESTINATION bin
           LIBRARY DESTINATION lib
           ARCHIVE DESTINATION lib/static
           PUBLIC_HEADER DESTINATION include/${ProjectName}
         )
  install(EXPORT "${ProjectName}Config" DESTINATION "lib/${ProjectName}")
  
#  INSTALL( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include
#           FILES_MATCHING PATTERN "*.h"
#           PATTERN "*/src"  EXCLUDE
#           PATTERN "*/test" EXCLUDE)
ENDMACRO()

MACRO (CcSetInstallConfig ProjectName )
  INSTALL( DIRECTORY    ${CMAKE_CURRENT_SOURCE_DIR}/config/
           DESTINATION  config/${ProjectName}
           PATTERN "*.xml" )
ENDMACRO()

MACRO (CcSetOSVersion ProjectName )
  SET_TARGET_PROPERTIES(  ${ProjectName}
                          PROPERTIES
                          VERSION ${CCOS_VERSION}
                          SOVERSION ${CCOS_VERSION})
ENDMACRO()

MACRO( CcSetFiltersByFolders )
  foreach(FILE ${ARGN})
    STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" FILETEMP ${FILE})
    STRING(REPLACE "/src/" "/" FILETEMP ${FILETEMP})
    STRING(REPLACE "/" "\\" FILETEMP ${FILETEMP})
    GET_FILENAME_COMPONENT( DIRNAME ${FILETEMP} DIRECTORY)
    if( "${DIRNAME}" STREQUAL "src")
      SOURCE_GROUP( "" FILES ${FILE})
    else()
      SOURCE_GROUP( "${DIRNAME}" FILES ${FILE})
    endif()
  ENDFOREACH()
ENDMACRO()

MACRO( CcLoadGuiSettings )
  if(DEFINED MSVC)
    # For Windows set Subsystem to Windows (/SUBSYSTEM:CONSOLE was set before)
    string(REPLACE "/SUBSYSTEM:CONSOLE" "/SUBSYSTEM:WINDOWS" CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
    # keep entry point on main(argc, argv)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup ")
  endif(DEFINED MSVC)
ENDMACRO( CcLoadGuiSettings )

MACRO( CcTargetIncludeDirs ProjectName )
  foreach(DIR ${ARGN})
    LIST(APPEND DIRS ${DIR} )
    target_include_directories(${ProjectName} PUBLIC $<BUILD_INTERFACE:${DIR}> )
  ENDFOREACH()
  target_include_directories( ${ProjectName} PUBLIC
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/${ProjectName}> )
ENDMACRO()
