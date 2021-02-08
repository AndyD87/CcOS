#pragma once 

#include <ntifs.h>

#include <CcWString.h>
#include "CcKernelModule.h"

struct CcKernelModuleSHARED _CcKernelModuleContext
{
public:
  PDRIVER_OBJECT  pDriverObject;  //!< Object from driver entry
  PUNICODE_STRING pRegistryPath;  //!< Register path from driver entry
  NTSTATUS        oDriverStatus;  //!< Global state of driver.
};

CCEXTERNC float _fltused;

//! NTDevice InitPath
#define NT_DEVICE_INIT                  L"\\Device\\"
