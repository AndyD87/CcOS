###############################################################################
# Add Source Files
file (GLOB SOURCE_FILES
      "*.cpp"
      "*.h"
)

###############################################################################
# Add already defined files
set(CCGUI_CCWINDOW_IMPLEMENTED TRUE PARENT_SCOPE)
set(CCGUI_CCWIDGET_IMPLEMENTED TRUE PARENT_SCOPE)

###############################################################################
# Setup Qt components in seperate library
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
add_library("CcGuiQt" ${CC_LINK_TYPE} "${CCOS_DIR}/Resources/dummy.c")
target_link_libraries( "CcGuiQt" PUBLIC Qt5::Core Qt5::Widgets )

add_to_gui_sources    ( ${SOURCE_FILES} )
add_to_gui_definitions( ${DEFINITIONS} )
add_to_gui_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} )
add_to_gui_libraries  ( "CcGuiQt" )

add_custom_command(
    TARGET "CcGuiQt" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Core>
        $<TARGET_FILE_DIR:CcGuiQt>
)

add_custom_command(
    TARGET "CcGuiQt" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Gui>
        $<TARGET_FILE_DIR:CcGuiQt>
)

add_custom_command(
    TARGET "CcGuiQt" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Widgets>
        $<TARGET_FILE_DIR:CcGuiQt>
)
