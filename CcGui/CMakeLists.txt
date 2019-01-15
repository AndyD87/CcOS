if(NOT DEFINED CCOS_CCGUI_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCGUI_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcGui )
  
  ########
  # Add Source Files
  ########
  file (GLOB GUI_SOURCE_FILES
        "*.h"
        "*.c"
        "*.cpp")
        
  file (GLOB SOURCE_Style_FILES
        "Style/*.c"
        "Style/*.cpp"
        "Style/*.h")

  file (GLOB SOURCE_BUTTONS_FILES
        "Buttons/*.h"
        "Buttons/*.c"
        "Buttons/*.cpp")

  set (GUI_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR})
        
  ########
  # Add Source Files
  ########
  file (GLOB SOURCE_WIDGETS_FILES
        "Widgets/*.h"
        "Widgets/*.c"
        "Widgets/*.cpp")

  macro (add_to_gui_sources)
      foreach (_src ${ARGN})
          CcListAppendOnce(GUI_SOURCE_FILES "${_src}")
      endforeach()
      set (GUI_SOURCE_FILES ${GUI_SOURCE_FILES} PARENT_SCOPE)
  endmacro()
  
  macro (add_to_gui_definitions)
      foreach (_src ${ARGN})
          CcListAppendOnce(GUI_DEFINITIONS "${_src}")
      endforeach()
      set (GUI_DEFINITIONS ${GUI_DEFINITIONS} PARENT_SCOPE)
  endmacro()
  
  macro (add_to_gui_includes)
      foreach (_src ${ARGN})
          CcListAppendOnce(GUI_INCLUDES "${_src}")
      endforeach()
      set (GUI_INCLUDES ${GUI_INCLUDES} PARENT_SCOPE)
  endmacro()
  
  macro (add_to_gui_libraries)
      foreach (_src ${ARGN})
          CcListAppendOnce(GUI_LIBRARIES "${_src}")
      endforeach()
      set (GUI_LIBRARIES ${GUI_LIBRARIES} PARENT_SCOPE)
  endmacro()

  add_to_gui_sources(${SOURCE_Style_FILES})
  add_to_gui_sources(${SOURCE_BUTTONS_FILES})
  add_to_gui_sources(${SOURCE_WIDGETS_FILES})

  macro (push_up_gui_vars)
    add_to_gui_sources     ( ${GUI_SOURCES} )
    add_to_gui_definitions ( ${GUI_DEFINITIONS} )
    add_to_gui_includes    ( ${GUI_INCLUDES} )
    add_to_gui_libraries   ( ${GUI_LIBRARIES} )
  endmacro()

  add_subdirectory(Subsystem)

  include_directories(${GUI_INCLUDES})
  add_definitions(${GUI_DEFINITIONS})
  
  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} GUI_SOURCE_FILES )
  endif()
                   
  add_library(${CURRENT_PROJECT} ${CC_LINK_TYPE}  ${GUI_SOURCE_FILES} )
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS")
  
  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                                CcKernel ${GUI_LIBRARIES} )
  
  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} ${GUI_INCLUDES})
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${GUI_SOURCE_FILES})
endif()
