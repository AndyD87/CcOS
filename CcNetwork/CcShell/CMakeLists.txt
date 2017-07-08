
set ( CURRENT_PROJECT CcShell )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "config/*.xml"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_library( ${CURRENT_PROJECT} ${CCOS_LINK_TYPE} ${SOURCE_FILES} )

SOURCE_GROUP( ""        FILES ${SOURCE_FILES})
SOURCE_GROUP( "config"  FILES "config/default.xml")

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../lib )

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcNetwork")

target_link_libraries ( ${CURRENT_PROJECT} PUBLIC
                              CcKernel
                              CcMedia )
    
#################
# Post configuration steps
#################
CcTargetIncludeDirs( ${CURRENT_PROJECT} )
CcSetInstall( ${CURRENT_PROJECT} )
CcSetInstallConfig ( ${CURRENT_PROJECT} )