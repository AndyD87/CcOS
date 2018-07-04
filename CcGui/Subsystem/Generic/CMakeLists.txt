########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "src/*.cpp"
      "*.h")

macro(remove_from_source)
  foreach (_src ${ARGN})
    list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${_src}")
  endforeach()
endmacro(remove_from_source)

if(NOT CCGUI_SUBSYSTEM STREQUAL "Generic")
  remove_from_source("CcGenericSubSystem.h")
  remove_from_source("src/CcGenericSubSystem.cpp")
endif(NOT CCGUI_SUBSYSTEM STREQUAL "Generic")

# now remove all previously defined files
if(DEFINED CCGUI_CCWINDOW_IMPLEMENTED)
  remove_from_source("src/CcWindow.cpp")
endif(DEFINED CCGUI_CCWINDOW_IMPLEMENTED)

if(DEFINED CCGUI_CCWIDGET_IMPLEMENTED)
  remove_from_source("src/CcWidget.cpp")
endif(DEFINED CCGUI_CCWIDGET_IMPLEMENTED)

if(DEFINED CCGUI_CCBUTTON_IMPLEMENTED)
  remove_from_source("src/CcButton.cpp")
endif(DEFINED CCGUI_CCBUTTON_IMPLEMENTED)

if(DEFINED CCGUI_CCPUSHBUTTON_IMPLEMENTED)
  remove_from_source("src/CcPushButton.cpp")
endif(DEFINED CCGUI_CCPUSHBUTTON_IMPLEMENTED)

add_to_gui_sources    ( ${SOURCE_FILES} )
add_to_gui_definitions( ${DEFINITIONS} )
add_to_gui_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} )
add_to_gui_libraries  ( )
