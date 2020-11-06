################################################################################
# Default Windows Config
################################################################################
message("- Default Configuration for WindowsKernel is selected")


find_package(WDK)
if(WDK_FOUND)
  set(WINDOWSKERNEL   TRUE)
  set(CCOS_NO_APPS    TRUE)
  set(CC_LINK_TYPE    STATIC)
  set(TOOLCHAIN_KERNEL_FILES ${CCOS_DIR}/CcKernel/Platform/Windows/Features/CppKernelRuntime.cpp)
else()
  message(FATAL_ERROR "WindowsKernel requires WDK for build")
endif()
