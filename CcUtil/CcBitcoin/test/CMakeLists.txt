set ( CURRENT_PROJECT CcBitcoinTest )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.c"
      "*.cpp"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_executable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS.Testing/CcUtil")

SOURCE_GROUP( "" FILES ${SOURCE_FILES})

target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                              CcBitcoin
                              CcKernel )
add_test( NAME    ${CURRENT_PROJECT}
          COMMAND ${EXECUTABLE_OUTPUT_PATH}/${CURRENT_PROJECT}
          WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} )