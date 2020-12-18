#pragma once 

#include "CcKernelModuleContext.h"

namespace NKernelModule
{

class CcRequest::CContext
{
public:
  PIRP                pIrp = nullptr;
  PIO_STACK_LOCATION  pStackLocation = nullptr;
};

}

CCEXTERNC float _fltused;
