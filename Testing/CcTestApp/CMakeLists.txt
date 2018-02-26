
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

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} 
                     ${CMAKE_CURRENT_SOURCE_DIR}/../../CcLib/CcUtil/ )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()
 
add_executable( ${CURRENT_PROJECT} ${SOURCE_FILES} ${HEADER_FILES} )

# To keep Header an Source together, display them in same Directory
# It will only work with IDE's wich are supporting this features
source_group( ""        FILES ${SOURCE_FILES})

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel
                              CcGui
                              CcSql
                              CcHttp
                              CcDocuments
                              )
                              
set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "Testing")
