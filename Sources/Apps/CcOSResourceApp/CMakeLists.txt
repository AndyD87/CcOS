set(CURRENT_PROJECT           CcOSResource    )
set(CURRENT_PROJECT_IDE_PATH  CcApps          )

########
# Add Source Files
########
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel
)

set_target_properties(${CURRENT_PROJECT} PROPERTIES FOLDER      "CcOS/${CURRENT_PROJECT_IDE_PATH}")

#################
# Post configuration steps
#################
CcTargetInstall( ${CURRENT_PROJECT} )
CcOSExeSettings(${CURRENT_PROJECT})
CcSetFiltersByFolders(${SOURCE_FILES})