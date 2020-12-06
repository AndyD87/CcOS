if(NOT DEFINED CCOS_CCKERNELMODULE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif( ${CCOS_CCKERNELMODULE_ACTIVE} GREATER ${CCOS_BUILDLEVEL} AND
        CCKERNEL_MODULE_AVAILABLE)

  set ( CURRENT_PROJECT           CcKernelModule )
  set ( CURRENT_PROJECT_IDE_PATH  ""    )

  #########
  ## Add Source Files
  #########
  file (GLOB KERNEL_SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../CcKernel/Types/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../CcKernel/Types/Hash/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../CcKernel/Network/CcCommonIps.cpp"
  )
  file (GLOB HEADER_FILES
        "${CMAKE_CURRENT_LIST_DIR}/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/../CcKernel/Types/*.h"
  )

  set(CCKERNELMODULE_OBJECT "/" CACHE INTERNAL "")

  set(CCKERNELMODULE_OBJECT_PATH "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"                        CACHE INTERNAL "")
  set(CCKERNELMODULE_OBJECT_NAME "lib${CURRENT_PROJECT}.a"                                  CACHE INTERNAL "")
  set(CCKERNELMODULE_OBJECT "${CCKERNELMODULE_OBJECT_PATH}/${CCKERNELMODULE_OBJECT_NAME}"   CACHE INTERNAL "")

  if(WINDOWS)
    file (GLOB WINDOWS_BUILD_FILES
          ${CMAKE_CURRENT_LIST_DIR}/Windows/*.cpp
    )
    include_directories(
      "${CMAKE_CURRENT_LIST_DIR}/Windows"
    )
    list(APPEND KERNEL_SOURCE_FILES ${WINDOWS_BUILD_FILES})
    list(APPEND KERNEL_MODULE_DEFINITIONS -DWINDOWSKERNEL)
    CcAppendLinkerFlags(/ignore:4210)
  elseif(LINUX)
    file (GLOB LINUX_BUILD_FILES
          ${CMAKE_CURRENT_LIST_DIR}/Linux/*.cpp
    )
    include_directories(
      "${CMAKE_CURRENT_LIST_DIR}/Linux"
    )
    list(APPEND KERNEL_SOURCE_FILES ${LINUX_BUILD_FILES})
    list(APPEND KERNEL_MODULE_DEFINITIONS -DLINUXKERNEL)
  endif()

  include_directories( ${CMAKE_CURRENT_LIST_DIR}              )
  include_directories( ${CCKERNEL_DIR}                        )
  include_directories( ${CCKERNEL_TYPES_DIR}                  )
  include_directories( ${CCKERNEL_MODULE_INCLUDE_DIRS}        )

  include_directories(
    "${CMAKE_CURRENT_LIST_DIR}/../CcKernel/Types"
  )

  CcAddDriverLibrary( ${CURRENT_PROJECT} ${KERNEL_SOURCE_FILES} ${HEADER_FILES})
  
  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  set_target_properties(${CURRENT_PROJECT} PROPERTIES LINKER_LANGUAGE CXX)
  target_compile_definitions(${CURRENT_PROJECT} PUBLIC ${KERNEL_MODULE_DEFINITIONS})

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${KERNEL_SOURCE_FILES} ${HEADER_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_LIST_DIR}/test)
endif()
