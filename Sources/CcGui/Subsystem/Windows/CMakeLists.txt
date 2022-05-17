########
# Add Source Files
########
set( SOURCE_FILES
        CcCWnd.cpp
        CcCWnd.h
        CcWindowsGuiButton.cpp
        CcWindowsGuiPixmap.cpp
        CcWindowsGuiPixmap.h
        CcWindowsGuiSubSystem.cpp
        CcWindowsGuiSubSystemMgmt.cpp
        CcWindowsGuiSubSystemMgmt.h
        CcWindowsGuiUtil.cpp
        CcWindowsGuiUtil.h
        CcWindowsGuiWidget.cpp
)

set(CCGUI_CCWIDGET_IMPLEMENTED TRUE PARENT_SCOPE)
set(CCGUI_CCBUTTON_IMPLEMENTED TRUE PARENT_SCOPE)

add_to_gui_sources    ( ${SOURCE_FILES} )
add_to_gui_definitions( ${DEFINITIONS} )
add_to_gui_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} )
add_to_gui_libraries  ( )
