#pragma once 

#include <ntifs.h>

#include <CcWString.h>
#include "CcKernelModule.h"

struct CcKernelModuleSHARED _CcKernelModuleContext
{
public:
  PDRIVER_OBJECT  pDriverObject;
  PUNICODE_STRING pRegistryPath;
  NTSTATUS        oDriverStatus;
};

CCEXTERNC float _fltused;


#define NT_DEVICE_INIT                  L"\\Device\\"