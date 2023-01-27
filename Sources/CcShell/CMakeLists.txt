set ( CURRENT_PROJECT           CcShell )
set ( CURRENT_PROJECT_IDE_PATH   )

########
# Add Header Files
########
set( HEADER_FILES
      CcShell.h
      CcShellBase.h
      Package/CPackage.h
      Package/CPackageInput.h
      Package/CPackageLogin.h
)

########
# Add Source Files
########
set( SOURCE_FILES
      ${HEADER_FILES}
      CcShell.cpp
      Package/CPackage.cpp
      Package/CPackageInput.cpp
      Package/CPackageLogin.cpp
)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

CcAddLibrary( ${CURRENT_PROJECT} ${CC_LINK_TYPE} ${SOURCE_FILES} )

set_target_properties( ${CURRENT_PROJECT} PROPERTIES FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
CcTargetHeaders( ${CURRENT_PROJECT} "${HEADER_FILES}")

target_link_libraries ( ${CURRENT_PROJECT} PUBLIC
                              CcKernel
)

#################
# Post configuration steps
#################
CcTargetIncludes( ${CURRENT_PROJECT} )
CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})
