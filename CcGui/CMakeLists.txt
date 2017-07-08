
set ( CURRENT_PROJECT CcGui )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.h"
      "src/*.c"
      "src/*.cpp")
      
file (GLOB SOURCE_STYLES_FILES
      "Styles/src/*.c"
      "Styles/src/*.cpp"
      "Styles/*.h")


########
# Add Source Files
########
file (GLOB SOURCE_BUTTONS_FILES
      "Buttons/*.h"
      "Buttons/src/*.c"
      "Buttons/src/*.cpp")
      
########
# Add Source Files
########
file (GLOB SOURCE_WIDGETS_FILES
      "Widgets/*.h"
      "Widgets/src/*.c"
      "Widgets/src/*.cpp")


include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
                     
add_library(${CURRENT_PROJECT} ${CCOS_LINK_TYPE}  ${SOURCE_FILES} 
                                ${SOURCE_STYLES_FILES} 
                                ${SOURCE_BUTTONS_FILES} 
                                ${SOURCE_WIDGETS_FILES} )

link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../lib )

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS")

SOURCE_GROUP( "Styles"  FILES ${SOURCE_STYLES_FILES})
SOURCE_GROUP( "Buttons" FILES ${SOURCE_BUTTONS_FILES})
SOURCE_GROUP( "Widgets" FILES ${SOURCE_WIDGETS_FILES})
SOURCE_GROUP( ""        FILES ${SOURCE_FILES})

target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                              CcKernel )

#################
# Post configuration steps
#################
CcTargetIncludeDirs( ${CURRENT_PROJECT} )
CcSetInstall( ${CURRENT_PROJECT} )