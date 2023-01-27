set(CURRENT_PROJECT           CcDhcpServer)
set(CURRENT_PROJECT_NAME      CcDhcpServer)
set(CURRENT_PROJECT_IDE_PATH  "CcApps"    )

########
# Add Source Files
########
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/CcDhcpServerApp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcDhcpServerApp.h
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel
                              CcDocuments
                              CcDhcp
)

set_target_properties(${CURRENT_PROJECT} PROPERTIES FOLDER      "CcOS/${CURRENT_PROJECT_IDE_PATH}")
set_target_properties(${CURRENT_PROJECT} PROPERTIES OUTPUT_NAME ${CURRENT_PROJECT_NAME})

#################
# Post configuration steps
#################
CcTargetInstall(${CURRENT_PROJECT})
CcOSExeSettings(${CURRENT_PROJECT})
