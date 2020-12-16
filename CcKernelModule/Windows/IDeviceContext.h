#include <wdm.h>
#include "IDevice.h"

namespace NKernelModule
{
class IDevice::CContext
{
public:
  NTSTATUS              iStatus = STATUS_SUCCESS;
  IDriver*              pDriver;
  PDEVICE_OBJECT        pDevice;
  IDevice*              pNextDevice = nullptr;
  CcList<CcConnection*> oConnections;
};

}