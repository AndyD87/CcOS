
set ( CURRENT_PROJECT CcMedia )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/CcImage.cpp"
      "src/CcVideo.cpp"
      "CcImage.h"
      "CcImageAbstract.h"
      "CcVideo.h")

set( LIBRARIES
        CcKernel )


if( CCOS_THIRDPARTY_LIBJPG )
  list(APPEND SOURCE_FILES "CcLibjpeg.h" )
  list(APPEND SOURCE_FILES "src/CcLibjpeg.cpp" )
  list(APPEND LIBRARIES jpeg)
endif()

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
         
if(WINDOWS)
  CcGenerateCcOSRcFileToCurrentDir(${CURRENT_PROJECT})
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
endif()
             
add_library(${CURRENT_PROJECT} ${CCOS_LINK_TYPE} ${SOURCE_FILES} )

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../lib )

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS")

# To keep Header an Source together, display them in same Directory
# It will only work with IDE's wich are supporting this features
SOURCE_GROUP( ""        FILES ${SOURCE_FILES})

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC ${LIBRARIES} )

#################
# Post configuration steps
#################
CcTargetIncludeDirs( ${CURRENT_PROJECT} )
CcSetInstall( ${CURRENT_PROJECT} )