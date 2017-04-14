
set ( CURRENT_PROJECT CcFtp )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "config/*.xml"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_library( ${CURRENT_PROJECT} ${LINK_TYPE} ${SOURCE_FILES} )

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcNetwork")

SOURCE_GROUP( ""        FILES ${SOURCE_FILES})
SOURCE_GROUP( "config"  FILES "config/default.xml")

target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                              CcKernel )

#################
# Post configuration steps
#################
CcTargetIncludeDirs( ${CURRENT_PROJECT} )
CcSetInstall( ${CURRENT_PROJECT} )
CcSetInstallConfig ( ${CURRENT_PROJECT} )