################################################################################
# Default Windows Config
################################################################################
message("- Default Configuration for Windows is selected")

################################################################################
# Setup default directories
################################################################################
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set( CMAKE_INSTALL_PREFIX       "C:/Program Files/CcOS/" )
endif()

if(NOT WINDOWS_KERNEL)
  set(WINDOWS TRUE)
else()
  find_package(WDK)
  if(WDK_FOUND)
    set(GENERIC       TRUE)
    set(CCOS_NO_APPS  TRUE)
    set(CC_LINK_TYPE  STATIC)
    add_definitions(-DGENERIC)
    set(TOOLCHAIN_KERNEL_FILES ${CCOS_DIR}/CcKernel/Platform/Windows/Features/CppKernelRuntime.cpp)
    
    CcAddExecutable("TestDriver" ${CCOS_DIR}/Resources/dummy.cpp)
    target_link_libraries("TestDriver" PUBLIC CcKernel)
  endif()
endif()