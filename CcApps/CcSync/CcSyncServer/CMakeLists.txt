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

add_executable( CcSyncServer ${SOURCE_FILES} ${HEADER_FILES} )

# To keep Header an Source together, display them in same Directory
# It will only work with IDE's wich are supporting this features
SOURCE_GROUP( ""        FILES ${SOURCE_FILES})

target_link_libraries ( CcSyncServer LINK_PUBLIC
                              CcKernel
                              CcSync
							                CcSql )

set_property( TARGET CcSyncServer PROPERTY FOLDER "CcOS/CcApps/CcSync")

#################
# Post configuration steps
#################
CcSetInstall( CcSyncServer )
