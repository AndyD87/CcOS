########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.c"
      "*.cpp"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_executable( CcDocumentsTest ${SOURCE_FILES} )

set_property( TARGET CcDocumentsTest PROPERTY FOLDER "CcOS/Testing/CcUtil")

SOURCE_GROUP( "" FILES ${SOURCE_FILES})

target_link_libraries ( CcDocumentsTest LINK_PRIVATE
                              CcDocuments
                              CcKernel )
add_test( NAME    CcDocumentTest
          COMMAND ${EXECUTABLE_OUTPUT_PATH}/CcDocumentsTest
          WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} )