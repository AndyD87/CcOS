
set ( CURRENT_PROJECT CcBitcoin )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcGenerateCcOSRcFileToCurrentDir(${CURRENT_PROJECT})
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
endif()
               
add_library(${CURRENT_PROJECT} ${CCOS_LINK_TYPE} ${SOURCE_FILES} )

# To keep Header an Source together, display them in same Directory
# It will only work with IDE's wich are supporting this features
SOURCE_GROUP( ""        FILES ${SOURCE_FILES})

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../lib )

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
