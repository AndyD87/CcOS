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
    CcAddDriverLibraryOverride( ${ProjectName} ${AddDriver_SOURCES})

    include(${CCKERNEL_MODULE_INCLUDE_DIRS}/CcKernelModuleLinux.cmake)
  endfunction()

  macro(CcAddDriverLibraryOverride ProjectName Sources)
    set(AddDriver_SOURCES ${Sources})
    foreach (_src ${ARGN})
      CcListAppendOnce(AddDriver_SOURCES "${_src}")
    endforeach()
    add_definitions(-DLINUXKERNEL)
    set(CMAKE_POSITION_INDEPENDENT_CODE FALSE)
    CcSetCCompilerFlags("-fno-builtin -nostdlib -fno-exceptions -fno-pie -mcmodel=kernel")
    CcSetCxxCompilerFlags("-std=c++11 -fno-builtin -nostdlib -fno-rtti -fno-exceptions -fno-pie -mcmodel=kernel")
    include_directories(${KERNELHEADERS_INCLUDE_DIRS})
    CcAddLibrary( ${ProjectName} STATIC ${AddDriver_SOURCES})
  endmacro()
endif(KERNELHEADERS_FOUND)
