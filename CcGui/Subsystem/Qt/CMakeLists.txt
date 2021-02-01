###############################################################################
# Add Source Files
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/CcQt.h
        ${CMAKE_CURRENT_LIST_DIR}/CcQtButton.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcQtSubSystem.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcQtText.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcQtWidget.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcQtWindow.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Qt/CStyleSheet.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Qt/CStyleSheet.h
)

if(QT_QMAKE_EXECUTABLE AND Qt5_DIR)
  find_package(Qt5 COMPONENTS Core Widgets)
elseif(QT_QMAKE_EXECUTABLE)
  find_package(Qt5 COMPONENTS Core Widgets)
elseif(LINUX)
  find_package(Qt5 COMPONENTS Core Widgets)
endif()

if(WINDOWS AND NOT Qt5_FOUND)
  set(QT5_VERSION 5.13.2.0)
  CcVisualStudioPostFix(VS_POSTFIX Release SHARED SHARED)

  # setup varibles for command
  set(QT5_FILENAME          "${VS_POSTFIX}.7z" )
  set(DOWNLOAD_URL          "https://coolcow.de/projects/ThirdParty/Qt5/binaries/${QT5_VERSION}/${QT5_FILENAME}")
  set(QT5_PATH              "${CC_CACHE_DIR}/Binaries/Qt5/${QT5_VERSION}/${VS_POSTFIX}" )

  CcDownloadAndExtract( "${QT5_FILENAME}"
                        "${QT5_PATH}"
                        "${DOWNLOAD_URL}")

  set(QT_QMAKE_EXECUTABLE ${QT5_PATH}/bin/qmake.exe     CACHE INTERNAL "Qt5 from cache")
  set(Qt5_DIR             ${QT5_PATH}/lib/cmake/Qt5     CACHE INTERNAL "Qt5 from cache")
  set(QT5Core_DIR         ${QT5_PATH}/lib/cmake/Qt5Core CACHE INTERNAL "Qt5 from cache")
  find_package(Qt5 COMPONENTS Core Widgets)
endif()

if(Qt5_FOUND)
  ###############################################################################
  # Add already defined files
  set(CCGUI_CCWINDOW_IMPLEMENTED TRUE)
  set(CCGUI_CCWIDGET_IMPLEMENTED TRUE)
  set(CCGUI_CCBUTTON_IMPLEMENTED TRUE)
  set(CCGUI_CCTEXT_IMPLEMENTED   TRUE)

  add_to_gui_sources    ( ${SOURCE_FILES} )
  add_to_gui_definitions( ${DEFINITIONS} )
  add_to_gui_includes   ( ${CMAKE_CURRENT_LIST_DIR} )
  add_to_gui_libraries  ( Qt5::Core Qt5::Widgets )
  if(WINDOWS)
    add_to_gui_libraries  ( uxtheme windowscodecs)
  endif(WINDOWS)

  add_to_gui_copies($<TARGET_FILE:Qt5::Core>)
  add_to_gui_copies($<TARGET_FILE:Qt5::Gui>)
  add_to_gui_copies($<TARGET_FILE:Qt5::Widgets>)
  if(WINDOWS)
    add_to_gui_copies("$<TARGET_FILE:Qt5::QWindowsIntegrationPlugin>#platforms")
  endif()
else()
  message("Qt not found, go back to Generic gui")
  set(CCGUI_SUBSYSTEM "Generic")
endif()
