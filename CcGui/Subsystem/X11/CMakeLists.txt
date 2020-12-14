########
# Add Source Files
########
set( SOURCE_FILES
        CcX11SubSystem.cpp
)

add_to_gui_sources    ( ${SOURCE_FILES} )
add_to_gui_definitions( ${DEFINITIONS} )
add_to_gui_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} ${X11_INCLUDES} )
add_to_gui_libraries  ( ${X11_LIBRARIES} )
