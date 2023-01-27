set(CURRENT_PROJECT CcHash)

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
                              CcKernel )

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcApps")

#################
# Post configuration steps
#################
CcTargetInstall( ${CURRENT_PROJECT} )
CcSetFiltersByFolders(${SOURCE_FILES})
