
set ( CURRENT_PROJECT CcDocuments )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "*.h"     
      "CcConfig/src/*.c"
      "CcConfig/src/*.cpp"
      "CcConfig/*.h"
      "CcHtml/src/*.c"
      "CcHtml/src/*.cpp"
      "CcHtml/*.h"
      "CcJson/src/*.c"
      "CcJson/src/*.cpp"
      "CcJson/*.h" 
      "CcXml/src/*.c"
      "CcXml/src/*.cpp"
      "CcXml/*.h"          )

set (DOCUMENTS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

include_directories( ${DOCUMENTS_INCLUDE_DIRS} )
                     
add_library(${CURRENT_PROJECT} ${CCOS_LINK_TYPE} ${SOURCE_FILES} )

FOREACH(FILE ${SOURCE_FILES})
  STRING(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/ "" FILETEMP ${FILE})
  STRING(REPLACE "/src/" "/" FILETEMP ${FILETEMP})
  get_filename_component( DIRNAME ${FILETEMP} DIRECTORY)
  if( "${DIRNAME}" STREQUAL "src")
    SOURCE_GROUP( "" FILES ${FILE})
  else()
    SOURCE_GROUP( "${DIRNAME}" FILES ${FILE})
  endif()
ENDFOREACH()

SOURCE_GROUP( ""        FILES ${SOURCE_FILES})

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcUtil")

target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                              CcKernel )

########
# Add an unity-test, if available
########
if( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
  add_subdirectory("test")
endif()

#################
# Post configuration steps
#################
CcTargetIncludeDirs( ${CURRENT_PROJECT} )
CcSetInstall( ${CURRENT_PROJECT} )
