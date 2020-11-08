if(NOT DEFINED CCOS_CCKERNELMODULE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCKERNELMODULE_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcKernelModule )
  set ( CURRENT_PROJECT_IDE_PATH  ""    )

  #########
  ## Add Source Files
  #########
  file (GLOB SOURCE_FILES
        "*.cpp"
        "../CcKernel/Types/*.cpp"
        "../CcKernel/Types/Hash/*.cpp"
        "../CcKernel/Network/CcCommonIps.cpp"
  )
  file (GLOB HEADER_FILES
        "*.h"
        "../CcKernel/Types/*.h"
  )
  file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/CcKernelModule.cpp "")
  foreach(SOURCE ${SOURCE_FILES})
    file (READ ${SOURCE} FILE_CONTENT)
    file (APPEND ${CMAKE_CURRENT_BINARY_DIR}/CcKernelModule.cpp "${FILE_CONTENT}")
  endforeach()

  set(CCKERNEL_TYPES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../CcKernel/Types  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../CcKernel        )
  include_directories( ${CCKERNEL_TYPES_DIR} )

  if(LINUXKERNEL)
    add_definitions(-DLINUXKERNEL)
    find_package(KernelHeaders REQUIRED)
    include_directories(${KERNELHEADERS_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/Linux)
    CcAppendCompilerFlags("-std=c++11 -fno-builtin -nostdlib -fno-rtti -fno-exceptions -fno-pie -mcmodel=kernel")
  elseif(WINDOWSKERNEL)
    add_definitions(-DWINDOWSKERNEL)
  endif()

  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)

  add_library( ${CURRENT_PROJECT}Single OBJECT ${CMAKE_CURRENT_BINARY_DIR}/CcKernelModule.cpp )
  add_library( ${CURRENT_PROJECT} OBJECT ${SOURCE_FILES} ${HEADER_FILES} )

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
    set(CCKERNELMODULE_OBJECT_PATH "CMakeFiles/CcKernelModuleSingle.dir/CcKernelModule.cpp.o")
    set(CCKERNELMODULE_OBJECT "${CMAKE_CURRENT_BINARY_DIR}/${CCKERNELMODULE_OBJECT_PATH}")
    message("CcKerneModule file ${CCKERNELMODULE_OBJECT}")
    add_subdirectory(Linux)

    message("$<TARGET_OBJECTS:${CURRENT_PROJECT}>")
  endif()

endif()
