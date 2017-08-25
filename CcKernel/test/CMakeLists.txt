########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.c"
      "*.cpp"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcGenerateCcOSRcFileToCurrentDir(${CURRENT_PROJECT})
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
endif()

add_executable( CcKernelTest ${SOURCE_FILES} )

set_property( TARGET CcKernelTest PROPERTY FOLDER "CcOS/Testing")

SOURCE_GROUP( "" FILES ${SOURCE_FILES})

target_link_libraries ( CcKernelTest LINK_PRIVATE CcKernel )
add_test( NAME    CcKernelTest
          COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CcKernelTest
          WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ) 