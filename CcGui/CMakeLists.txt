if(NOT DEFINED CCOS_CCGUI_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCGUI_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcGui )

  ########
  # Add Source Files
  ########
  file (GLOB GUI_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

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
  endmacro()

  macro (add_to_gui_definitions)
      foreach (_src ${ARGN})
          CcListAppendOnce(GUI_DEFINITIONS "${_src}")
      endforeach()
  endmacro()

  macro (add_to_gui_includes)
      foreach (_src ${ARGN})
          CcListAppendOnce(GUI_INCLUDES "${_src}")
      endforeach()
  endmacro()

  macro (add_to_gui_libraries)
      foreach (_src ${ARGN})
          CcListAppendOnce(GUI_LIBRARIES "${_src}")
      endforeach()
  endmacro()

  macro (add_to_gui_copies)
      foreach (_src ${ARGN})
          CcListAppendOnce(GUI_COPIES "${_src}")
      endforeach()
  endmacro()

  add_to_gui_sources(${SOURCE_Style_FILES})
  add_to_gui_sources(${SOURCE_BUTTONS_FILES})
  add_to_gui_sources(${SOURCE_WIDGETS_FILES})

  include(Subsystem/CMakeLists.txt)

  include_directories(${GUI_INCLUDES})
  add_definitions(${GUI_DEFINITIONS})

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} GUI_SOURCE_FILES )
  endif()

  add_library(${CURRENT_PROJECT} ${CC_LINK_TYPE}  ${GUI_SOURCE_FILES} )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PRIVATE
                                CcKernel ${GUI_LIBRARIES}
  )

  foreach (GUI_COPY ${GUI_COPIES})
    string(REGEX MATCH "#" DO_MATCH ${GUI_COPY})
    if(DO_MATCH)
      string(REPLACE "#" ";" NEW_GUI_COPIES ${GUI_COPY})
      list(GET NEW_GUI_COPIES 0 NEW_GUI_COPIES_FIRST )
      list(GET NEW_GUI_COPIES 1 NEW_GUI_COPIES_SECOND )
      add_custom_command(
          TARGET "CcGui" PRE_BUILD
          COMMAND ${CMAKE_COMMAND} -E make_directory
              $<TARGET_FILE_DIR:CcGui>/${NEW_GUI_COPIES_SECOND}
          COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${NEW_GUI_COPIES_FIRST}
              $<TARGET_FILE_DIR:CcGui>/${NEW_GUI_COPIES_SECOND}
      )
    else()
      add_custom_command(
          TARGET "CcGui" POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${GUI_COPY}
              $<TARGET_FILE_DIR:CcGui>
      )
    endif()
  endforeach()

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} ${GUI_INCLUDES})
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${GUI_SOURCE_FILES})
endif()
