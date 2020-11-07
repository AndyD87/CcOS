if(NOT DEFINED CCOS_CCKERNELMODULE_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_CCKERNELMODULE_ACTIVE} GREATER ${CCOS_BUILDLEVEL})

  set ( CURRENT_PROJECT           CcKernelModule )
  set ( CURRENT_PROJECT_IDE_PATH  ""    )

  #########
  ## Add Source Files
  #########
  file (GLOB SOURCE_FILES
        "*.cpp"
        "*.h"
  )

  include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../CcKernel        )
  include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../CcKernel/Types  )

  if(LINUXKERNEL)
    find_package(KernelHeaders REQUIRED)
    include_directories(${KERNELHEADERS_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/Linux)
    CcAppendCompilerFlags("-std=c++11 -fno-builtin -nostdlib -fno-rtti -fno-exceptions -fno-pie -mcmodel=kernel")
  endif()

  CcAddLibrary( ${CURRENT_PROJECT} OBJECT ${SOURCE_FILES} )

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
    set(CCKERNELMODULE_OBJECT_PATH "CMakeFiles/CcKernelModule.dir/CcKernelModule.cpp.o")#$<TARGET_FILE:CcKernelModule>)
    set(CCKERNELMODULE_OBJECT "${CMAKE_CURRENT_BINARY_DIR}/${CCKERNELMODULE_OBJECT_PATH}")
    message("CcKerneModule file ${CCKERNELMODULE_OBJECT}")
    add_subdirectory(Linux)
  endif()

endif()
