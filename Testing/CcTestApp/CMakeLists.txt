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

#source_group("Header" FILES ${HEADER_FILES})

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} 
                     ${CMAKE_CURRENT_SOURCE_DIR}/../../CcLib/CcUtil/ )

link_directories( ${LIBRARY_OUTPUT_PATH} ${LIBRARY_OUTPUT_PATH}/Debug )

add_executable( CcTestApp ${SOURCE_FILES} ${HEADER_FILES} )

target_link_libraries ( CcTestApp LINK_PUBLIC
                CcKernel
                              CcGui
                              CcSql
                              CcHttp
                              CcDocuments
                              )
                              
set_property( TARGET CcTestApp PROPERTY FOLDER "Testing")
