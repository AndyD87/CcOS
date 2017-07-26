########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.c"
      "*.cpp"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_executable( CcKernelTest ${SOURCE_FILES} )

set_property( TARGET CcKernelTest PROPERTY FOLDER "CcOS/Testing")

SOURCE_GROUP( "" FILES ${SOURCE_FILES})

target_link_libraries ( CcKernelTest LINK_PRIVATE CcKernel )
add_test( NAME    CcKernelTest
          COMMAND ${EXECUTABLE_OUTPUT_PATH}/CcKernelTest
          WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} ) 