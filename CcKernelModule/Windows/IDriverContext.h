#include <wdm.h>
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
  PDRIVER_OBJECT  pDriverObject;
  CcWString       sRegistryPath;
};
}