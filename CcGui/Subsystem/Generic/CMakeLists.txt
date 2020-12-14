########
# Add Source Files
########
set( SOURCE_FILES    
      CcButton.cpp
      CcFont.cpp
      CcFont.h
      CcGenericSubSystem.cpp
      CcText.cpp
      CcWidget.cpp
      CcWindow.cpp
      Font/CcFontFile.cpp
      Font/CcFontFile.h
      Font/CcFontStatic.cpp
      Font/CcFontStatic.h
      Font/CcFontTableGlyph.cpp
      Font/CcFontTableGlyph.h
      Font/CcFontTableHead.cpp
      Font/CcFontTableHead.h
      Font/CcFontTableLoca.cpp
      Font/CcFontTableLoca.h
      Font/CcFontTableMaxp.h
      Font/CcFontTableName.cpp
      Font/CcFontTableName.h
)

########
# Add Resource Files
########
set( RESOURCE_FILES
      Resources/CcFontRectangleBitstreamVeraSans.h
)

macro(remove_from_source)
  foreach (_src ${ARGN})
    list(REMOVE_ITEM SOURCE_FILES "${_src}")
  endforeach()
endmacro(remove_from_source)

if(NOT CCGUI_SUBSYSTEM STREQUAL "Generic")
  remove_from_source("${CMAKE_CURRENT_LIST_DIR}/CcGenericSubSystem.cpp")
endif(NOT CCGUI_SUBSYSTEM STREQUAL "Generic")

# now remove all previously defined files
if(DEFINED CCGUI_CCWINDOW_IMPLEMENTED)
  remove_from_source("${CMAKE_CURRENT_LIST_DIR}/CcWindow.cpp")
endif(DEFINED CCGUI_CCWINDOW_IMPLEMENTED)

if(DEFINED CCGUI_CCWIDGET_IMPLEMENTED)
  remove_from_source("${CMAKE_CURRENT_LIST_DIR}/CcWidget.cpp")
endif(DEFINED CCGUI_CCWIDGET_IMPLEMENTED)

if(DEFINED CCGUI_CCBUTTON_IMPLEMENTED)
  remove_from_source("${CMAKE_CURRENT_LIST_DIR}/CcButton.cpp")
endif(DEFINED CCGUI_CCBUTTON_IMPLEMENTED)

if(DEFINED CCGUI_CCPUSHBUTTON_IMPLEMENTED)
  remove_from_source("${CMAKE_CURRENT_LIST_DIR}/CcButton.cpp")
endif(DEFINED CCGUI_CCPUSHBUTTON_IMPLEMENTED)

if(DEFINED CCGUI_CCTEXT_IMPLEMENTED)
  remove_from_source("${CMAKE_CURRENT_LIST_DIR}/CcText.cpp")
endif(DEFINED CCGUI_CCTEXT_IMPLEMENTED)

add_to_gui_sources    ( ${SOURCE_FILES} ${RESOURCE_FILES})
add_to_gui_definitions( ${DEFINITIONS} )
add_to_gui_includes   ( ${CMAKE_CURRENT_LIST_DIR} )
add_to_gui_libraries  ( )
