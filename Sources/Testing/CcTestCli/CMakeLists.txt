
set ( CURRENT_PROJECT CcTestCli )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.c"
      "*.cpp")

########
# Add Source Files
########
file (GLOB HEADER_FILES
      "*.h")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}  )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

CcAddGuiExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} ${HEADER_FILES} )

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel
                              CcDocuments
)

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "Testing")

#################
# Post configuration steps
#################
CcOSExeSettings(${CURRENT_PROJECT})
CcSetFiltersByFolders(${SOURCE_FILES})
CcTargetInstall(${CURRENT_PROJECT})
