
########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

link_directories( ${EXECUTABLE_OUTPUT_PATH} )

add_executable( CcSqliteTest ${SOURCE_FILES} )

SOURCE_GROUP( "" FILES ${SOURCE_FILES})

target_link_libraries ( CcSqliteTest LINK_PUBLIC
                              CcKernel
                              CcGui
                              CcSql
                              CcShell
                              #CcDism
                              )
                              
set_property( TARGET CcSqliteTest PROPERTY FOLDER "Testing")
