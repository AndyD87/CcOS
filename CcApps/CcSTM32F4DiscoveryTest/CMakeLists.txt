
set(CURRENT_PROJECT CcSTM32F4DiscoveryTest)

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_executable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel )
                              
set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcApps")

#################
# Post configuration steps
#################
CcSetInstall( ${CURRENT_PROJECT} )
