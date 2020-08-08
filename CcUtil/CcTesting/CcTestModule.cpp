/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcTestModule
 */
#include "CcTestModule.h"
#include "CcTesting.h"

#include <windows.h>
#include <process.h>

//CcTestModule* g_ppModule;
//
//void Delllll()
//{
//  CCDELETE(g_ppModule);
//}
//
//BOOL WINAPI DllMain(
//  HINSTANCE /*hinstDLL*/,  // handle to DLL module
//  DWORD fdwReason,     // reason for calling function
//  LPVOID /*lpReserved*/)  // reserved
//{
//  IKernel oKernel;
//  // Perform actions based on the reason for calling.
//  switch (fdwReason)
//  {
//    case DLL_PROCESS_ATTACH:
//      CCNEW(g_ppModule, CcTestModule, oKernel);
//      atexit(Delllll);
//      // Initialize once for each new process.
//      // Return FALSE to fail DLL load.
//      break;
//
//    case DLL_THREAD_ATTACH:
//      // Do thread-specific initialization.
//      break;
//
//    case DLL_THREAD_DETACH:
//      // Do thread-specific cleanup.
//      break;
//
//    case DLL_PROCESS_DETACH:
//      // Perform any necessary cleanup.
//      break;
//  }
//  return TRUE;  // Successful DLL_PROCESS_ATTACH.
//}

CCEXTERNC CcTestingSHARED IModule* IModule_Create(const IKernel& oKernel)
{
  CCNEWTYPE(pModule, CcTestModule, oKernel);
  return pModule;
}

CCEXTERNC CcTestingSHARED void IModule_Remove(IModule* pModule)
{
  CCDELETE(pModule);
}

CcTestModule::CcTestModule(const IKernel& oKernel) :
  IModule(oKernel)
{
}

CcTestModule::~CcTestModule()
{
  m_oKernel.pBaseObject = nullptr;
}

CcStatus CcTestModule::init()
{
  CcStatus oStatus;
  return oStatus;
}

CcStatus CcTestModule::deinit()
{
  CcStatus oStatus;
  return oStatus;
}
