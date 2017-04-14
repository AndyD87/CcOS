########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "*.h")
      
set(CURRENT_TARGET ccsha256)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_executable( ${CURRENT_TARGET} ${SOURCE_FILES} )

target_link_libraries ( ${CURRENT_TARGET} LINK_PUBLIC
                              CcKernel )
                              
set_property( TARGET ${CURRENT_TARGET} PROPERTY FOLDER "CcApps")

#################
# Post configuration steps
#################
CcSetInstall( ${CURRENT_TARGET} )
