
set ( CURRENT_PROJECT CcBitcoin )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.c"
      "src/*.cpp"
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
                     
add_library(${CURRENT_PROJECT} ${LINK_TYPE} ${SOURCE_FILES} )

SOURCE_GROUP( ""        FILES ${SOURCE_FILES})

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../lib )

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcUtil")

target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                              CcKernel
                              CcCrypto )

#################
# Post configuration steps
#################
CcTargetIncludeDirs( ${CURRENT_PROJECT} )
CcSetInstall( ${CURRENT_PROJECT} )