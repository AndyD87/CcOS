if(NOT DEFINED CCOS_CCKERNELMODULE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif( ${CCOS_CCKERNELMODULE_ACTIVE} GREATER ${CCOS_BUILDLEVEL} AND
        CCKERNEL_MODULE_AVAILABLE)

  set ( CURRENT_PROJECT           CcKernelModule )
  set ( CURRENT_PROJECT_IDE_PATH  ""    )

  #########
  ## Add Source Files
  #########
  file (GLOB KERNEL_SOURCE_FILES
        "*.cpp"
        "../CcKernel/Types/*.cpp"
        "../CcKernel/Types/Hash/*.cpp"
        "../CcKernel/Network/CcCommonIps.cpp"
  )
  file (GLOB HEADER_FILES
        "*.h"
        "../CcKernel/Types/*.h"
  )

  set(CONCATENATED_FILE ${CMAKE_CURRENT_BINARY_DIR}/CcKernelModuleConcatenated.cpp)

  file (GLOB BUILD_FILES
        ${CONCATENATED_FILE}
  )

  include(ConcatenateSources.cmake)

  add_custom_command(
    OUTPUT ${CONCATENATED_FILE}
    DEPENDS ${KERNEL_SOURCE_FILES} ${HEADER_FILES}
    COMMAND ${CMAKE_COMMAND}  -D CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR} -D CONCATENATED_FILE=${CONCATENATED_FILE} -P ConcatenateSources.cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )

  add_custom_target ( ${CURRENT_PROJECT}ConcatenateSources ALL
                      DEPENDS ${CONCATENATED_FILE}
                      SOURCES ${KERNEL_SOURCE_FILES} ${HEADER_FILES}
  )

  include_directories( ${CMAKE_CURRENT_LIST_DIR}              )
  include_directories( ${CCKERNEL_DIR}                        )
  include_directories( ${CCKERNEL_TYPES_DIR}                  )
  include_directories( ${CCKERNEL_MODULE_INCLUDE_DIRS}        )

  CcAddDriverLibrary( ${CURRENT_PROJECT} ${BUILD_FILES} ${HEADER_FILES})
  add_dependencies(${CURRENT_PROJECT} ${CURRENT_PROJECT}ConcatenateSources)

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")
  set_target_properties(${CURRENT_PROJECT} PROPERTIES LINKER_LANGUAGE CXX)
  set(CCKERNELMODULE_OBJECT "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/lib${CURRENT_PROJECT}.a" CACHE INTERNAL "")

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${KERNEL_SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_LIST_DIR}/test)

endif()
