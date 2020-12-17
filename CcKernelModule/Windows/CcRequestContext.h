#pragma once 

#include "CcKernelModuleContext.h"

namespace NKernelModule
{

class CcRequest::CContext
{
public:
  PIRP                pIrp;
  PIO_STACK_LOCATION  pStackLocation;
};

}

CCEXTERNC float _fltused;
