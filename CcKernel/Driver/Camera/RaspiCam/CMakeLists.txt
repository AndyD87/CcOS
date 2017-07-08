########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.cpp"
      "src/*.c" )


########
# Add Source Files
########
file (GLOB HEADER_FILES
      "*h" )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
                     
add_library(driverLinuxV4L ${CCOS_LINK_TYPE} ${SOURCE_FILES} ${HEADER_FILES} )

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../../lib )

set_property( TARGET driverLinuxV4L PROPERTY FOLDER "CcOS")

target_link_libraries ( driverLinuxV4L LINK_PRIVATE
                              CcKernel
                              libv4l-0 )

set_property( TARGET CcMedia PROPERTY FOLDER "CcOS/CcDriver/Camera")

target_include_directories( driverLinuxV4L PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
