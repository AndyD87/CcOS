
set ( CURRENT_PROJECT GenericTest )

########
# Add Source Files
########
set(SOURCE_FILES
  main.cpp
  GenericApp.cpp 
  GenericService.cpp 
)

########
# Add Source Files
########
set(HEADER_FILES
  GenericApp.h 
  GenericService.h 
)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}  )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

CcAddGuiExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} ${HEADER_FILES} )

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel
)

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "Testing")

#################
# Post configuration steps
#################
CcOSExeSettings(${CURRENT_PROJECT})
CcSetFiltersByFolders(${SOURCE_FILES})
CcTargetInstall(${CURRENT_PROJECT})
