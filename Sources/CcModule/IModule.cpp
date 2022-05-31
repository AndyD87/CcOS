/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Redirect new and delete to main application.
 *            This file must be included once on every Module
 */
#include "CcKernel.h"
#include "IModule.h"

#ifdef WINDOWS
  #include <windows.h>
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

CcVector<IModule*>*  IModule::s_pInstances = nullptr;

IModule::~IModule()
{
  if (s_pInstances)
  {
    s_pInstances->removeItem(this);
    if (s_pInstances->size() == 0)
    {
#if !defined(CC_STATIC) && !defined(LINUX)
      // Remove if all remove
      CcKernel::setInterface(nullptr);
#endif
    }
  }
}

void IModule::initStatic()
{
  if (!s_pInstances)
  {
    CCNEW(s_pInstances, CcVector<IModule*>);
  }
}

void IModule::deinitStatic()
{
  if (s_pInstances)
  {
    CcVector<IModule*>*  pInstances = s_pInstances;
    // Avoid multiple unloads
    s_pInstances = nullptr;
    for (IModule* pModule : *pInstances)
    {
      pModule->m_oUnloadEvent.call(pModule);
    }
    CCDELETE(pInstances);
  }
}

void IModule::setKernel(const IKernel& oKernel)
{
  if (s_pInstances)
  {
    if (s_pInstances->size() == 0)
    {
      #ifndef CC_STATIC
        CcKernel::setInterface(oKernel);
      #else
        CCUNUSED(oKernel);
      #endif
    }
    s_pInstances->append(this);
  }
}
