#include <ntddk.h>
#include <CcWString.h>

class CcKernelModuleContext
{
public:
  PDRIVER_OBJECT  pDriverObject;
  PUNICODE_STRING pRegistryPath;
  NTSTATUS        oDriverStatus;
};