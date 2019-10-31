########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.cpp"
      "*.h")

########
# Add Resource Files
########
file (GLOB RESOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/Resources/*.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/Resources/*.c"
      "${CMAKE_CURRENT_LIST_DIR}/Resources/*.h")

macro(remove_from_source)
  foreach (_src ${ARGN})
    list(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${_src}")
  endforeach()
endmacro(remove_from_source)

if(NOT CCGUI_SUBSYSTEM STREQUAL "Generic")
  remove_from_source("CcGenericSubSystem.h")
  remove_from_source("CcGenericSubSystem.cpp")
endif(NOT CCGUI_SUBSYSTEM STREQUAL "Generic")

# now remove all previously defined files
if(DEFINED CCGUI_CCWINDOW_IMPLEMENTED)
  remove_from_source("CcWindow.cpp")
endif(DEFINED CCGUI_CCWINDOW_IMPLEMENTED)

if(DEFINED CCGUI_CCWIDGET_IMPLEMENTED)
  remove_from_source("CcWidget.cpp")
endif(DEFINED CCGUI_CCWIDGET_IMPLEMENTED)

if(DEFINED CCGUI_CCBUTTON_IMPLEMENTED)
  remove_from_source("CcButton.cpp")
endif(DEFINED CCGUI_CCBUTTON_IMPLEMENTED)

if(DEFINED CCGUI_CCPUSHBUTTON_IMPLEMENTED)
  remove_from_source("CcButton.cpp")
endif(DEFINED CCGUI_CCPUSHBUTTON_IMPLEMENTED)

if(DEFINED CCGUI_CCTEXT_IMPLEMENTED)
  remove_from_source("CcText.cpp")
endif(DEFINED CCGUI_CCTEXT_IMPLEMENTED)

add_to_gui_sources    ( ${SOURCE_FILES} ${RESOURCE_FILES})
add_to_gui_definitions( ${DEFINITIONS} )
add_to_gui_includes   ( ${CMAKE_CURRENT_SOURCE_DIR} )
add_to_gui_libraries  ( )
