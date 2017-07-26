########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp")


########
# Add Source Files
########
file (GLOB HEADER_FILES
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

link_directories( ${LIBRARY_OUTPUT_PATH} ${LIBRARY_OUTPUT_PATH}/Debug )

add_executable( CcSyncClientSvc ${SOURCE_FILES} ${HEADER_FILES} )

SOURCE_GROUP( ""        FILES ${SOURCE_FILES})

target_link_libraries ( CcSyncClientSvc LINK_PUBLIC
                              CcKernel
                              CcSync
                              CcSql )
                              
set_property( TARGET CcSyncClientSvc PROPERTY FOLDER "CcOS/CcApps/CcSync")

#################
# Post configuration steps
#################
CcSetInstall( CcSyncClientSvc )
