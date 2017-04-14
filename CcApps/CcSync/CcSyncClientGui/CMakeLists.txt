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

add_executable( CcSyncClientGui ${SOURCE_FILES} ${HEADER_FILES} )

SOURCE_GROUP( ""        FILES ${SOURCE_FILES})

target_link_libraries ( CcSyncClientGui LINK_PUBLIC
                              CcGui
                              CcKernel
                              CcSync )
                              
set_property( TARGET CcSyncClientGui PROPERTY FOLDER "CcApps/CcSync")

#################
# Post configuration steps
#################
CcSetInstall( CcSyncClientGui )
