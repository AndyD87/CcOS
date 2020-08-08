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
 * @page      CcKernel
 * @subpage   IModuleMemoryRedirect
 *
 * @page      IModuleMemoryRedirect
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Redirect new and delete to main application.
 *            This file must be included once on every Module
 */
#ifndef H_IModuleMemoryRedirect_H_
#define H_IModuleMemoryRedirect_H_

#include "CcKernel.h"
#include "IModule.h"
#include "IKernel.h"
#include <cstdlib>

IModule*        IModule::s_pInstance = nullptr;
CcEventHandler  IModule::s_oUnloadEvent;

IModule::IModule(const IKernel& oKernel)
{
  atexit(IModule::unload);
  if (s_pInstance == nullptr)
  {
    #ifndef CC_STATIC
      CcKernel::setInterface(oKernel.pBaseObject);
    #else 
      CCUNUSED(oKernel);
    #endif  
    s_pInstance = this;
  }
}

IModule::~IModule()
{
  // Remove only first instance
  if (s_pInstance == this)
  {
    s_pInstance = nullptr;
    #ifndef CC_STATIC
      CcKernel::setInterface(nullptr);
    #endif  
  }
}

void IModule::registerOnUnload(const CcEvent& oUnloadEvent)
{
  s_oUnloadEvent.append(oUnloadEvent);
}

void IModule::unregisterOnUnload(CcObject* pUnregister)
{
  s_oUnloadEvent.removeObject(pUnregister);
}

void IModule::unload()
{
  if (s_pInstance)
  {
    s_oUnloadEvent.call(s_pInstance);
    s_pInstance = nullptr;
  }
}

#endif // H_IModuleMemoryRedirect_H_
