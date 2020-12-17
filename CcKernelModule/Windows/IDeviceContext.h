#pragma once

#include <wdm.h>
#include "IDevice.h"

namespace NKernelModule
{
class IDevice::CContext : DEVICE_OBJECT
{
public:
  NTSTATUS                iStatus;
  IDriver*                pDriver;
  IDevice*                pDevice;
  IDevice*                pNextDevice;

  CcList<CcConnection*>* getConnections()
  { if (!pConnections) CCNEW(pConnections, CcList<CcConnection*>); return pConnections; }

private:
  CcList<CcConnection*>*  pConnections;
};

}