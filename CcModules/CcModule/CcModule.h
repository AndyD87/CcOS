#include "CcBase.h"

#ifdef WINDOWS

extern BOOL WINAPI DllMain
(
  HINSTANCE hinstDLL,  // handle to DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpReserved    // reserved
);

#else

class StartupAndCleanup
{
public:
  StartupAndCleanup();
  ~StartupAndCleanup();
};

extern StartupAndCleanup g_oStartupAndCleanup;

#endif

extern void CcDllMain();