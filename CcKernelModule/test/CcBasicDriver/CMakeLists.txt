set ( CURRENT_PROJECT           CcBasicDriver )
set ( CURRENT_PROJECT_IDE_PATH  ""    )

#########
## Add Source Files
#########
file (GLOB SOURCE_FILES
      "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
)
file (GLOB HEADER_FILES
      "${CMAKE_CURRENT_LIST_DIR}/*.h"
)

if(WINDOWS)
  CcOSGenerateRcFileToCurrentDir(${CURRENT_PROJECT} SOURCE_FILES )
endif()

CcAddDriver( ${CURRENT_PROJECT} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries ( ${CURRENT_PROJECT} LINK_PUBLIC
                              CcKernelModule
)

set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/CcKernelModule/${CURRENT_PROJECT_IDE_PATH}")
set_target_properties(${CURRENT_PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

#################
# Post configuration steps
#################
CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
CcOSExeSettings(${CURRENT_PROJECT})
