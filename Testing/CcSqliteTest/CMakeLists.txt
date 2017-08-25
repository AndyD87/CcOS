
set ( CURRENT_PROJECT CcSqliteTest )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

link_directories( ${EXECUTABLE_OUTPUT_PATH} )

if(WINDOWS)
  CcGenerateCcOSRcFileToCurrentDir(${CURRENT_PROJECT})
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
endif()
 
add_executable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

SOURCE_GROUP( "" FILES ${SOURCE_FILES})

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel
                              CcGui
                              CcSql
                              CcShell
                              #CcDism
                              )
                              
set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/Testing")
