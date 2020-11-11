set(LINUX           TRUE)


find_package(KernelHeaders)
if(KERNELHEADERS_FOUND)
  message("-- LinuxHeaders available")

  set(CCKERNEL_DIR        ${CMAKE_CURRENT_LIST_DIR}/../../CcKernel        )
  set(CCKERNEL_TYPES_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../CcKernel/Types  )
  set(CCKERNELMODULE_DIR  ${CMAKE_CURRENT_LIST_DIR}/../../CcKernelModule  )
  set(CCKERNEL_MODULE_AVAILABLE     TRUE)
  set(CCKERNEL_MODULE_INCLUDE_DIRS  ${CMAKE_CURRENT_LIST_DIR}/Linux)

  function(CcAddDriverOverride ProjectName Sources)
    add_definitions(-DLINUXKERNEL)
    CcSetCompilerFlags("-std=c++11 -fno-builtin -nostdlib -fno-rtti -fno-exceptions -fno-pie -mcmodel=kernel")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Linux)

    CcAddLibrary( ${ProjectName} OBJECT ${Sources})

    set(CCKERNELMODULE_OBJECT_PATH "CMakeFiles/CcKernelModule.dir/CcKernelModuleConcatenated.cpp.o")
    set(CCKERNELMODULE_OBJECT "${CMAKE_CURRENT_BINARY_DIR}/${CCKERNELMODULE_OBJECT_PATH}")
  endfunction()



  function(CcAddDriverLibraryOverride ProjectName Sources)
    add_definitions(-DLINUXKERNEL)
    CcAppendCompilerFlags("-std=c++11 -fno-builtin -nostdlib -fno-rtti -fno-exceptions -fno-pie -mcmodel=kernel")

    CcAddLibrary( ${ProjectName} STATIC ${Sources})

    message(${CCKERNEL_MODULE_INCLUDE_DIRS}/CcKernelModuleLinux.cmake)
    include(${CCKERNEL_MODULE_INCLUDE_DIRS}/CcKernelModuleLinux.cmake)
  endfunction()
endif(KERNELHEADERS_FOUND)
