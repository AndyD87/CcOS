########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.cpp"
      "*.h")

add_to_gui_sources    ( ${SOURCE_FILES} )
add_to_gui_definitions( ${DEFINITIONS} )
add_to_gui_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} ${X11_INCLUDES} )
add_to_gui_libraries  ( ${X11_LIBRARIES} )
