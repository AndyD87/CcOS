
set ( CURRENT_PROJECT CcTftp )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "config/*.xml"
      "*.h")
 
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcGenerateCcOSRcFileToCurrentDir(${CURRENT_PROJECT})
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
endif()
 
add_library( ${CURRENT_PROJECT} ${CCOS_LINK_TYPE} ${SOURCE_FILES})

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../lib )

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcNetwork")


target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                              CcKernel )

#################
# Post configuration steps
#################
CcSetInstall( ${CURRENT_PROJECT} )
CcSetInstallConfig ( ${CURRENT_PROJECT} )
CcSetFiltersByFolders( ${SOURCE_FILES} )
