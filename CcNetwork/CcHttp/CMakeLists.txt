
set ( CURRENT_PROJECT CcHttp )

########
# Add Source Files
########
file (GLOB HTTP_SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "config/*.xml"
      "*.h")

macro (add_to_http_sources)
    foreach (_src ${ARGN})
        list (APPEND HTTP_SOURCE_FILES "${_src}")
    endforeach()
    set (HTTP_SOURCE_FILES ${HTTP_SOURCE_FILES} PARENT_SCOPE)
endmacro()

########
# Add Subdirecotry Files
########
add_subdirectory(HttpProvider/)

set( DEFINITIONS "" )
set( LIBRARIES  CcKernel
                CcMedia )
if( CCOS_THIRDPARTY_OPENSSL )
  list( APPEND LIBRARIES CcSsl)
  list( APPEND DEFINITIONS -DCCSSL_ENABLED)
endif()

add_definitions( ${DEFINITIONS} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcGenerateCcOSRcFileToCurrentDir(${CURRENT_PROJECT})
  list(APPEND HTTP_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
endif()
 
add_library( ${CURRENT_PROJECT} ${CCOS_LINK_TYPE} ${HTTP_SOURCE_FILES} )

SOURCE_GROUP( ""        FILES ${HTTP_SOURCE_FILES})
SOURCE_GROUP( "config"  FILES "config/default.xml")

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../lib )

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcNetwork")

target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE ${LIBRARIES} )

#################
# Post configuration steps
#################
CcTargetIncludeDirs( ${CURRENT_PROJECT} )
CcSetInstall( ${CURRENT_PROJECT} )
CcSetInstallConfig ( ${CURRENT_PROJECT} )