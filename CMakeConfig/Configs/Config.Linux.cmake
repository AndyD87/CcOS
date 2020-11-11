set(LINUX           TRUE)


find_package(KernelHeaders)
if(KERNELHEADERS_FOUND)
  message("-- LinuxHeaders available")
  set(CCKERNEL_MODULE_AVAILABLE TRUE)

  function(CcAddDriverOverride ProjectName Sources)
    add_definitions(-DLINUXKERNEL)
    CcAppendCompilerFlags("-std=c++11 -fno-builtin -nostdlib -fno-rtti -fno-exceptions -fno-pie -mcmodel=kernel")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Linux)

    CcAddLibrary( ${ProjectName} OBJECT ${Sources})

    set(CCKERNELMODULE_OBJECT_PATH "CMakeFiles/CcKernelModule.dir/CcKernelModuleConcatenated.cpp.o")
    set(CCKERNELMODULE_OBJECT "${CMAKE_CURRENT_BINARY_DIR}/${CCKERNELMODULE_OBJECT_PATH}")
  endfunction()

  function(CcAddDriverLibraryOverride ProjectName Sources)
    add_definitions(-DLINUXKERNEL)
    CcAppendCompilerFlags("-std=c++11 -fno-builtin -nostdlib -fno-rtti -fno-exceptions -fno-pie -mcmodel=kernel")

    CcAddLibrary( ${ProjectName} OBJECT ${Sources})

    set(CCKERNELMODULE_OBJECT_PATH "CMakeFiles/CcKernelModule.dir/CcKernelModuleConcatenated.cpp.o")
    set(CCKERNELMODULE_OBJECT "${CMAKE_CURRENT_BINARY_DIR}/${CCKERNELMODULE_OBJECT_PATH}")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Linux)
  endfunction()
endif(KERNELHEADERS_FOUND)
