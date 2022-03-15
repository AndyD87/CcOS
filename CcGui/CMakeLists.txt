if(NOT DEFINED CCOS_CCGUI_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCGUI_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcGui )

  ########
  # Add Source Files
  ########
  set( GUI_SOURCE_FILES
              ${CMAKE_CURRENT_LIST_DIR}/CcCross.cpp
              ${CMAKE_CURRENT_LIST_DIR}/CcGuiApplication.cpp
              ${CMAKE_CURRENT_LIST_DIR}/CcGuiGlobals.cpp
              ${CMAKE_CURRENT_LIST_DIR}/CcMenu.cpp
              ${CMAKE_CURRENT_LIST_DIR}/CcMenuItem.cpp
              ${CMAKE_CURRENT_LIST_DIR}/CcMenuReverse.cpp
              ${CMAKE_CURRENT_LIST_DIR}/CcPainter.cpp
              ${CMAKE_CURRENT_LIST_DIR}/CcTaskbar.cpp
              ${CMAKE_CURRENT_LIST_DIR}/CcTaskbarItem.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Buttons/CcCalibrationButton.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Buttons/CcCloseButton.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Buttons/CcCrossButton.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Buttons/CcMaximizeButton.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Buttons/CcMinimizeButton.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Style/CcStyle.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Style/CcStyleButton.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Style/CcStyleText.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Style/CcStyleWidget.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Style/CcStyleWindow.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Widgets/CcTableWidget.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Widgets/CcTableWidgetCell.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Widgets/CcTableWidgetRow.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Widgets/CcTitlebar.cpp
              ${CMAKE_CURRENT_LIST_DIR}/Widgets/Private/CcTableWidgetColumnData.cpp
  )

  ########
  # Add Header Files
  ########
  set( GUI_HEADER_FILES
              ${CMAKE_CURRENT_LIST_DIR}/CcButton.h
              ${CMAKE_CURRENT_LIST_DIR}/CcCross.h
              ${CMAKE_CURRENT_LIST_DIR}/CcCrossButton.h
              ${CMAKE_CURRENT_LIST_DIR}/CcGui.h
              ${CMAKE_CURRENT_LIST_DIR}/CcGuiApplication.h
              ${CMAKE_CURRENT_LIST_DIR}/CcGuiGlobals.h
              ${CMAKE_CURRENT_LIST_DIR}/CcGuiSubsystem.h
              ${CMAKE_CURRENT_LIST_DIR}/CcMenu.h
              ${CMAKE_CURRENT_LIST_DIR}/CcMenuItem.h
              ${CMAKE_CURRENT_LIST_DIR}/CcMenuReverse.h
              ${CMAKE_CURRENT_LIST_DIR}/CcPainter.h
              ${CMAKE_CURRENT_LIST_DIR}/CcTaskbar.h
              ${CMAKE_CURRENT_LIST_DIR}/CcTaskbarItem.h
              ${CMAKE_CURRENT_LIST_DIR}/CcText.h
              ${CMAKE_CURRENT_LIST_DIR}/CcWidget.h
              ${CMAKE_CURRENT_LIST_DIR}/CcWindow.h

              ${CMAKE_CURRENT_LIST_DIR}/Buttons/CcButtons.h
              ${CMAKE_CURRENT_LIST_DIR}/Buttons/CcCalibrationButton.h
              ${CMAKE_CURRENT_LIST_DIR}/Buttons/CcCloseButton.h
              ${CMAKE_CURRENT_LIST_DIR}/Buttons/CcMaximizeButton.h
              ${CMAKE_CURRENT_LIST_DIR}/Buttons/CcMinimizeButton.h

              ${CMAKE_CURRENT_LIST_DIR}/Style/CcStyle.h
              ${CMAKE_CURRENT_LIST_DIR}/Style/CcStyleButton.h
              ${CMAKE_CURRENT_LIST_DIR}/Style/CcStyleText.h
              ${CMAKE_CURRENT_LIST_DIR}/Style/CcStyleWidget.h
              ${CMAKE_CURRENT_LIST_DIR}/Style/CcStyleWindow.h

              ${CMAKE_CURRENT_LIST_DIR}/Widgets/CcTableWidget.h
              ${CMAKE_CURRENT_LIST_DIR}/Widgets/CcTableWidgetCell.h
              ${CMAKE_CURRENT_LIST_DIR}/Widgets/CcTableWidgetRow.h
              ${CMAKE_CURRENT_LIST_DIR}/Widgets/CcTitlebar.h
              ${CMAKE_CURRENT_LIST_DIR}/Widgets/CcWidgets.h
              ${CMAKE_CURRENT_LIST_DIR}/Widgets/Private/CcTableWidgetColumnData.h
  )

  set (GUI_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR})

  macro (add_to_gui_sources)
      foreach (_src ${ARGN})
          CcListAppendOnce(GUI_SOURCE_FILES "${_src}")
      endforeach()
  endmacro()

  macro (add_to_gui_headers)
      foreach (_src ${ARGN})
          CcListAppendOnce(GUI_HEEADER_FILES "${_src}")
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

  include(Subsystem/CMakeLists.txt)

  include_directories(${GUI_INCLUDES})
  add_definitions(${GUI_DEFINITIONS})

  if(WINDOWS)
    CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} GUI_SOURCE_FILES )
  endif()

  CcAddLibrary(${CURRENT_PROJECT} ${CC_LINK_TYPE}  ${GUI_SOURCE_FILES} ${GUI_HEADER_FILES} )

  set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS")
  set_target_properties( ${CURRENT_PROJECT} PROPERTIES PUBLIC_HEADER "${GUI_HEADER_FILES}")

  target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                                CcKernel ${GUI_LIBRARIES}
  )

  foreach (GUI_COPY ${GUI_COPIES})
    string(REGEX MATCH "#" DO_MATCH ${GUI_COPY})
    if(DO_MATCH)
      string(REPLACE "#" ";" NEW_GUI_COPIES ${GUI_COPY})
      list(GET NEW_GUI_COPIES 0 NEW_GUI_COPIES_FIRST )
      list(GET NEW_GUI_COPIES 1 NEW_GUI_COPIES_SECOND )
      if("${CC_LINK_TYPE}" STREQUAL "STATIC")
        add_custom_command(
            TARGET "CcGui" PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                $<TARGET_FILE_DIR:CcGui>/../../bin/${NEW_GUI_COPIES_SECOND}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${NEW_GUI_COPIES_FIRST}
                $<TARGET_FILE_DIR:CcGui>/../../bin/${NEW_GUI_COPIES_SECOND}
        )
      else()
        add_custom_command(
            TARGET "CcGui" PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                $<TARGET_FILE_DIR:CcGui>/${NEW_GUI_COPIES_SECOND}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${NEW_GUI_COPIES_FIRST}
                $<TARGET_FILE_DIR:CcGui>/${NEW_GUI_COPIES_SECOND}
        )
      endif()
    else()
      if("${CC_LINK_TYPE}" STREQUAL "STATIC")
        add_custom_command(
            TARGET "CcGui" POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${GUI_COPY}
                $<TARGET_FILE_DIR:CcGui>/../../bin
        )
      else()
        add_custom_command(
            TARGET "CcGui" POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${GUI_COPY}
                $<TARGET_FILE_DIR:CcGui>
        )
      endif()
    endif()
  endforeach()

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} ${GUI_INCLUDES})
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE  ${GUI_SOURCE_FILES} ${GUI_HEADER_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_LIST_DIR}/test)
endif()
