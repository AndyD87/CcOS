if(NOT DEFINED CCOS_CCKERNELMODULE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCKERNELMODULE_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

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
  
  if(LINUXKERNEL)
    add_definitions(-DLINUXKERNEL)
    find_package(KernelHeaders REQUIRED)
    include_directories(${KERNELHEADERS_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/Linux)
    CcAppendCompilerFlags("-std=c++11 -fno-builtin -nostdlib -fno-rtti -fno-exceptions -fno-pie -mcmodel=kernel")
  elseif(WINDOWSKERNEL)
    file (GLOB WINDOWS_KERNEL_SOURCE_FILES
          "Windows/*.cpp"
    )
    list(APPEND BUILD_FILES ${WINDOWS_KERNEL_SOURCE_FILES} ${BUILD_FILES})
    add_definitions(-DWINDOWSKERNEL)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Windows)
  endif()

  add_custom_command(
    OUTPUT ${CONCATENATED_FILE}
    DEPENDS ${SOURCE_FILES} ${HEADER_FILES}
    COMMAND ${CMAKE_COMMAND}  -D CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
                              -D CONCATENATED_FILE=${CONCATENATED_FILE}
                              -P ConcatenateSources.cmake
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

  add_custom_target ( ${CURRENT_PROJECT}ConcatenateSources ALL
                      DEPENDS ${CONCATENATED_FILE}
                      SOURCES ${KERNEL_SOURCE_FILES} ${HEADER_FILES}
  )

  include(ConcatenateSources.cmake)

  set(CCKERNEL_TYPES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../CcKernel/Types  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../CcKernel        )
  include_directories( ${CCKERNEL_TYPES_DIR} )

  CcAddLibrary( ${CURRENT_PROJECT} OBJECT ${BUILD_FILES} ${HEADER_FILES})
  add_dependencies(${CURRENT_PROJECT} ${CURRENT_PROJECT}ConcatenateSources)

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/${CURRENT_PROJECT_IDE_PATH}")

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  CcOSLibSettings(${CURRENT_PROJECT} TRUE TRUE ${SOURCE_FILES})

  ########
  # Add an unity-test, if available
  ########
  CcAddTestDir(${CMAKE_CURRENT_SOURCE_DIR}/test)

  if(LINUXKERNEL)
    set(CCKERNELMODULE_OBJECT_PATH "CMakeFiles/CcKernelModule.dir/CcKernelModuleConcatenated.cpp.o")
    set(CCKERNELMODULE_OBJECT "${CMAKE_CURRENT_BINARY_DIR}/${CCKERNELMODULE_OBJECT_PATH}")
    add_subdirectory(Linux)
  endif()

endif()
