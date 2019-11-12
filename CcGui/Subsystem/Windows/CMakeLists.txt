########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.cpp"
      "*.h")

set(CCGUI_CCWIDGET_IMPLEMENTED TRUE PARENT_SCOPE)
set(CCGUI_CCBUTTON_IMPLEMENTED TRUE PARENT_SCOPE)

add_to_gui_sources    ( ${SOURCE_FILES} )
add_to_gui_definitions( ${DEFINITIONS} )
add_to_gui_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} )
add_to_gui_libraries  ( )
