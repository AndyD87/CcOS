#include <ntddk.h>
#include <CcWString.h>

struct _CcKernelModuleContext
{
public:
  PDRIVER_OBJECT  pDriverObject;
  PUNICODE_STRING pRegistryPath;
  NTSTATUS        oDriverStatus;
};