
set ( CURRENT_PROJECT CcSqliteTest )

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

source_group( "" FILES ${SOURCE_FILES})

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernel
                              CcSql
                              CcShell
                              CcTesting )

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/Testing/CcUtil")

#################
# Post configuration steps
#################
CcOSExeSettings(${CURRENT_PROJECT})