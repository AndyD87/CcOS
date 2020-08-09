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
#include "CcKernel.h"
#include "IModule.h"
#include "IKernel.h"
#include <cstdlib>

CcVector<IModule*>*  IModule::s_pInstances = nullptr;

IModule::IModule(const IKernel& oKernel)
{
  if(s_pInstances->size() == 0)
  {
    #ifndef CC_STATIC
      CcKernel::setInterface(oKernel.pBaseObject);
    #else
      CCUNUSED(oKernel);
    #endif
    atexit(IModule::unload);
  }
  s_pInstances->append(this);
}

IModule::~IModule()
{
  s_pInstances->removeItem(this);
  #ifndef CC_STATIC
    // Remove if all remove
    if(s_pInstances->size() == 0)
    {
        CcKernel::setInterface(nullptr);
    }
  #endif
}

void IModule::registerOnUnload(const CcEvent& oUnloadEvent)
{
  m_oUnloadEvent.append(oUnloadEvent);
}

void IModule::unregisterOnUnload(CcObject* pUnregister)
{
  m_oUnloadEvent.removeObject(pUnregister);
}

void IModule::initStatic()
{
  CCNEW(s_pInstances ,CcVector<IModule*>);
}

void IModule::deinitStatic()
{
  CCDELETE(s_pInstances);
}

void IModule::unload()
{
  if(s_pInstances)
  {
    for(IModule* pModule : *s_pInstances)
    {
      pModule->m_oUnloadEvent.call(pModule);
    }
    s_pInstances->clear();
  }
}
