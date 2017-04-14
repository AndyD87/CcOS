
set ( CURRENT_PROJECT CcSsl )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "config/*.xml"
      "*.h")
 
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

link_directories( ${OPENSLL_LIB_DIR} )

add_library( ${CURRENT_PROJECT} ${LINK_TYPE} ${SOURCE_FILES})

SOURCE_GROUP( ""        FILES ${SOURCE_FILES})
SOURCE_GROUP( "config"  FILES "config/default.xml")

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcNetwork")

target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                                CcKernel
                                ${OPENSSL_LIBRARIES} )
 

#################
# Post configuration steps
#################
CcTargetIncludeDirs( ${CURRENT_PROJECT} ${OPENSLL_INCLUDE_DIR} )
CcSetInstall( ${CURRENT_PROJECT} )
CcSetInstallConfig ( ${CURRENT_PROJECT} )
