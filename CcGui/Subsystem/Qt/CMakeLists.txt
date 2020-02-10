###############################################################################
# Add Source Files
file (GLOB SOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/*.h"
      "${CMAKE_CURRENT_LIST_DIR}/Qt/*.cpp"
      "${CMAKE_CURRENT_LIST_DIR}/Qt/*.h"
)

###############################################################################
# Add already defined files
set(CCGUI_CCWINDOW_IMPLEMENTED TRUE)
set(CCGUI_CCWIDGET_IMPLEMENTED TRUE)
set(CCGUI_CCBUTTON_IMPLEMENTED TRUE)
set(CCGUI_CCTEXT_IMPLEMENTED   TRUE)

###############################################################################
# Setup Qt components in seperate library

if(QT_QMAKE_EXECUTABLE AND Qt5_DIR)
  find_package(Qt5 COMPONENTS Core Widgets)
elseif(LINUX)
  find_package(Qt5 COMPONENTS Core Widgets)
endif()

if(WINDOWS AND NOT Qt5_FOUND)
  set(QT5_VERSION 5.13.2.0)
  CcVisualStudioPostFix(VS_POSTFIX Release SHARED SHARED)

  # setup varibles for command
  set(QT5_FILENAME          "${VS_POSTFIX}.7z" )
  set(DOWNLOAD_URL          "http://coolcow.de/projects/ThirdParty/Qt5/binaries/${QT5_VERSION}/${QT5_FILENAME}")
  set(QT5_PATH              "${CC_CACHE_DIR}/Qt5/${QT5_VERSION}/${VS_POSTFIX}" )

  CcDownloadAndExtract( "${QT5_FILENAME}"
                        "${QT5_PATH}"
                        "${DOWNLOAD_URL}")

  set(QT_QMAKE_EXECUTABLE ${QT5_PATH}/bin/qmake.exe)
  set(Qt5_DIR             ${QT5_PATH}/lib/cmake/Qt5)
  set(QT5Core_DIR         ${QT5_PATH}/lib/cmake/Qt5Core)
  find_package(Qt5 COMPONENTS Core Widgets)
endif()

if(Qt5_FOUND)
  add_to_gui_sources    ( ${SOURCE_FILES} )
  add_to_gui_definitions( ${DEFINITIONS} )
  add_to_gui_includes   ( ${CMAKE_CURRENT_LIST_DIR} )
  add_to_gui_libraries  ( Qt5::Core Qt5::Widgets )

  add_to_gui_copies($<TARGET_FILE:Qt5::Core>)
  add_to_gui_copies($<TARGET_FILE:Qt5::Gui>)
  add_to_gui_copies($<TARGET_FILE:Qt5::Widgets>)
  if(WINDOWS)
    add_to_gui_copies("$<TARGET_FILE:Qt5::QWindowsIntegrationPlugin>#platforms")
  endif()
endif()
