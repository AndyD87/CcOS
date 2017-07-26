########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "*.h")
      

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_executable( CcSyncClientCli ${SOURCE_FILES} )

link_directories( ${LIBRARY_OUTPUT_PATH} ${LIBRARY_OUTPUT_PATH}/Debug )

target_link_libraries ( CcSyncClientCli LINK_PUBLIC
                              CcKernel
                              CcSync
                              CcSql )
                              
set_property( TARGET CcSyncClientCli PROPERTY FOLDER "CcOS/CcApps/CcSync")

#################
# Post configuration steps
#################
CcSetInstall( CcSyncClientCli )
