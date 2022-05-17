
set ( CURRENT_PROJECT CcNetworkApp )

########
# Add Source Files
########
file (GLOB SOURCE_FILES
      "*.c"
      "*.cpp"
      "*.h")


include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

CcAddExecutable( ${CURRENT_PROJECT} ${SOURCE_FILES} )

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel
                              CcFtp
                              CcHttp
                              CcSql
                              CcShell
                              CcFtp
                              CcTftp
                              CcDhcp
                              CcDocuments
                              )

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "Testing")

#################
# Post configuration steps
#################
CcOSExeSettings(${CURRENT_PROJECT})
CcSetFiltersByFolders(${SOURCE_FILES})