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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Redirect new and delete to main application.
 *            This file must be included once on every Module
 */
#include "CcKernel.h"
#include "IModule.h"
#include "CcModule.h"

CcVector<IModule*>*  IModule::s_pInstances = nullptr;

IModule::~IModule()
{
  if (s_pInstances)
  {
    s_pInstances->removeItem(this);
    if (s_pInstances->size() == 0)
    {
#ifndef CC_STATIC
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
      CcKernel::setInterface(oKernel.pBaseObject);
#else
      CCUNUSED(oKernel);
#endif
    }
    s_pInstances->append(this);
  }
}
