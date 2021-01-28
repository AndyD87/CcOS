#include "CcBase.h"

#ifdef WINDOWS

extern BOOL WINAPI DllMain
(
  HINSTANCE hinstDLL,  // handle to DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpReserved    // reserved
);

#else

/**
 * @brief Use this static class for handling library load and
 *        and unload process.
 */
class StartupAndCleanup
{
public:
  /**
   * @brief Lib entry
   */
  StartupAndCleanup();

  /**
   * @brief Lib exit
   */
  ~StartupAndCleanup();
};

extern StartupAndCleanup g_oStartupAndCleanup;

#endif

extern void CcDllMain();
