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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcGGphotoCamera
 */

#include "CcV4LModule.h"
#include "CcKernel.h"

CCEXTERNC IModule* IModule_Create(const IKernel& oKernel)
{
  CCNEWTYPE(pModule, CcV4LModule, oKernel);
  return pModule;
}

CCEXTERNC void IModule_Remove(IModule* pModule)
{
  CCDELETE(pModule);
}

CcV4LModule::CcV4LModule(const IKernel& oKernel) :
  IModule(oKernel)
{
}

CcV4LModule::~CcV4LModule()
{
}

CcStatus CcV4LModule::init()
{
  m_oDriver.entry();
  return true;
}

CcStatus CcV4LModule::deinit()
{
  m_oDriver.unload();
  return true;
}
