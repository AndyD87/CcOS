if(NOT DEFINED CCOS_CCGUI_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCGUI_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  set ( CURRENT_PROJECT CcGui )

  ########
  # Add Source Files
  ########
  set( GUI_SOURCE_FILES
         CcCross.cpp
         CcGuiApplication.cpp
         CcGuiGlobals.cpp
         CcMenu.cpp
         CcMenuItem.cpp
         CcMenuReverse.cpp
         CcPainter.cpp
         CcTaskbar.cpp
         CcTaskbarItem.cpp
         Buttons/CcCalibrationButton.cpp
         Buttons/CcCloseButton.cpp
         Buttons/CcCrossButton.cpp
         Buttons/CcMaximizeButton.cpp
         Buttons/CcMinimizeButton.cpp
         Style/CcStyle.cpp
         Style/CcStyleButton.cpp
         Style/CcStyleText.cpp
         Style/CcStyleWidget.cpp
         Style/CcStyleWindow.cpp
         Widgets/CcTableWidget.cpp
         Widgets/CcTableWidgetCell.cpp
         Widgets/CcTableWidgetRow.cpp
         Widgets/CcTitlebar.cpp
         Widgets/Private/CcTableWidgetColumnData.cpp
  )

  ########
  # Add Header Files
  ########
  set( GUI_HEADER_FILES
         CcButton.h
         CcCross.h
         CcCrossButton.h
         CcFont.h
         CcGui.h
         CcGuiApplication.h
         CcGuiGlobals.h
         CcGuiSubsystem.h
         CcMenu.h
         CcMenuItem.h
         CcMenuReverse.h
         CcPainter.h
         CcTaskbar.h
         CcTaskbarItem.h
         CcText.h
         CcWidget.h
         CcWindow.h

         Buttons/CcButtons.h
         Buttons/CcCalibrationButton.h
         Buttons/CcCloseButton.h
         Buttons/CcMaximizeButton.h
         Buttons/CcMinimizeButton.h

         Style/CcStyle.h
         Style/CcStyleButton.h
         Style/CcStyleText.h
         Style/CcStyleWidget.h
         Style/CcStyleWindow.h

         Widgets/CcTableWidget.h
         Widgets/CcTableWidgetCell.h
         Widgets/CcTableWidgetRow.h
         Widgets/CcTitlebar.h
         Widgets/CcWidgets.h
         Widgets/Private/CcTableWidgetColumnData.h
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
  CcTargetHeaders( ${CURRENT_PROJECT} "${GUI_HEADER_FILES}")

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
  CcTargetIncludes( ${CURRENT_PROJECT} ${GUI_INCLUDES})
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE  ${GUI_SOURCE_FILES} ${GUI_HEADER_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_LIST_DIR}/test)
endif()
