set(CURRENT_PROJECT           CcScpiCli           )
set(CURRENT_PROJECT_IDE_PATH  CcApps              )

########
# Add Source Files
########
set( SOURCE_FILES
      ${CMAKE_CURRENT_LIST_DIR}/CcScpiCli.cpp
      ${CMAKE_CURRENT_LIST_DIR}/CcScpiCli.h
      ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel
                              CcScpi
)

set_target_properties(${CURRENT_PROJECT} PROPERTIES FOLDER      "CcOS/${CURRENT_PROJECT_IDE_PATH}")
set_target_properties(${CURRENT_PROJECT} PROPERTIES OUTPUT_NAME  CcScpiCli)

#################
# Post configuration steps
#################
CcTargetInstall(${CURRENT_PROJECT})
CcOSExeSettings(${CURRENT_PROJECT})
CcSetFiltersByFolders(${SOURCE_FILES})
