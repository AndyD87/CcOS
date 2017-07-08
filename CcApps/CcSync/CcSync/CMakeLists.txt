
set ( CURRENT_PROJECT CcSync)

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
SOURCE_GROUP( "config"  FILES "config/default.Client.xml")
SOURCE_GROUP( "config"  FILES "config/default.Server.xml")

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../lib )
 
target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcSsl
                              CcKernel
                              CcDocuments
                              CcSql )

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcApps/${CURRENT_PROJECT}")


#################
# Post configuration steps
#################
CcTargetIncludeDirs( ${CURRENT_PROJECT} )
CcSetInstall( ${CURRENT_PROJECT} )
#CcSetInstallConfig ( ${CURRENT_PROJECT} )