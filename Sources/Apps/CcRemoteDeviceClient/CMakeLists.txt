set(CURRENT_PROJECT           CcRemoteDeviceClient)
set(CURRENT_PROJECT_IDE_PATH  "CcApps"    )

########
# Add Source Files
########
set( SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceClientApp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/CcRemoteDeviceClientApp.h
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

CcAddGuiExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel
                              CcRemoteDevice
)

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
set_target_properties(${CURRENT_PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

#################
# Post configuration steps
#################
CcTargetInstall(${CURRENT_PROJECT})
CcOSExeSettings(${CURRENT_PROJECT})

