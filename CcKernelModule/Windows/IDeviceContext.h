#pragma once

#include "CcKernelModuleContext.h"
#include "IDevice.h"

namespace NKernelModule
{
class IDevice::CContext : public DEVICE_OBJECT
{
public:
  IDriver*                pDriver;
  IDevice*                pDevice;
  IDevice*                pNextDevice;

  CcList<CcConnection*>* getConnections()
  { if (!pConnections) CCNEW(pConnections, CcList<CcConnection*>); return pConnections; }

private:
  CcList<CcConnection*>*  pConnections;
};

}