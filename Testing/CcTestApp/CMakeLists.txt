
set ( CURRENT_PROJECT CcTestApp )

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

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT})
  list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CcOSVersion.rc")
endif()
 
add_executable( ${CURRENT_PROJECT} ${SOURCE_FILES} ${HEADER_FILES} )

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel
                              CcGui
                              CcSql
                              CcHttp
                              CcDocuments
                              )
                              
set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "Testing")
