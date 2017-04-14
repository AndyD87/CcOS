########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} 
                     ${CMAKE_CURRENT_SOURCE_DIR}/../../CcLib/CcNetwork/
                     ${CMAKE_CURRENT_SOURCE_DIR}/../CcLib/CcUtil/ )

add_executable( CcNetworkApp ${SOURCE_FILES} )

SOURCE_GROUP( ""        FILES ${SOURCE_FILES})

target_link_libraries ( CcNetworkApp LINK_PUBLIC
                              CcKernel
                              CcGui
                              CcFtp
                              CcHttp
                              CcSql
                              CcShell
                              CcFtp
                              CcTftp
                              CcDocuments
                              )
                              
set_property( TARGET CcNetworkApp PROPERTY FOLDER "Testing")

#################
# Post configuration steps
#################
CcSetInstall( CcNetworkApp )
