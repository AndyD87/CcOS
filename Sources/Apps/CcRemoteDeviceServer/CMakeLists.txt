set(CURRENT_PROJECT           CcRemoteDeviceServer)
set(CURRENT_PROJECT_IDE_PATH  CcApps              )

########
# Add Source Files
########
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceServerApp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceServerApp.h
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceServerService.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceServerService.h
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel
                              CcRemoteDevice 
)

set_target_properties(${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)

#################
# Post configuration steps
#################
CcTargetInstall(${CURRENT_PROJECT})
CcOSExeSettings(${CURRENT_PROJECT})
CcSetFiltersByFolders(${SOURCE_FILES})
