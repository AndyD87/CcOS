#include "CcBase.h"
#include "IModule.h"

#ifdef WINDOWS
#include <windows.h>
#include "CcModule.h"

BOOL WINAPI DllMain
(
  HINSTANCE CCUNUSED_PARAM(hinstDLL),   // handle to DLL module
  DWORD fdwReason,                      // reason for calling function
  LPVOID CCUNUSED_PARAM(lpReserved)     // reserved
)
{
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
