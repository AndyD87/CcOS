#include "CcBase.h"
#include "IModule.h"

#ifdef WINDOWS
#include <windows.h>
#include "CcModule.h"

BOOL WINAPI DllMain
(
  HINSTANCE hinstDLL,  // handle to DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpReserved    // reserved
)
{
  CCUNUSED(hinstDLL);
  CCUNUSED(lpReserved);
  // Perform actions based on the reason for calling.
  switch (fdwReason)
  {
  case DLL_PROCESS_ATTACH:
    // Initialize once for each new process.
    // Return FALSE to fail DLL load.
    IModule::initStatic();
    break;
  case DLL_THREAD_ATTACH:
    // Do thread-specific initialization.
    break;
  case DLL_THREAD_DETACH:
    // Do thread-specific cleanup.
    break;
  case DLL_PROCESS_DETACH:
    // Perform any necessary cleanup.
    IModule::deinitStatic();
    break;
  }
  return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

#else
#include "CcModule.h"

StartupAndCleanup::StartupAndCleanup()
{
  IModule::initStatic();
}

StartupAndCleanup::~StartupAndCleanup()
{
  IModule::deinitStatic();
}

StartupAndCleanup g_oStartupAndCleanup;

#endif

void CcDllMain()
{
  #ifdef WINDOWS2
    DllMain(0, 0, 0);
  #else
      
  #endif // WINDOWS
}
