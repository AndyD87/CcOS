#pragma once

#include "CcKernelModuleContext.h"
#include "CcWString.h"
#include "IDriver.h"

namespace NKernelModule
{
class IDriver::CContext
{
public:
  CContext()
  {}

  IDriver*        pDriver;
};
}
