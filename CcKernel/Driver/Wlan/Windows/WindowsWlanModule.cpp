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
 * @brief     Implementation of Class WindowsWlanModule
 */

#include "WindowsWlanModule.h"
#include "CcKernel.h"

CCEXTERNC WindowsWlanSHARED IModule* IModule_Create(const IKernel& oKernel)
{
  CCNEWTYPE(pModule, WindowsWlanModule, oKernel);
  return pModule;
}

CCEXTERNC WindowsWlanSHARED void IModule_Remove(IModule* pModule)
{
  CCDELETE(pModule);
}

WindowsWlanModule::WindowsWlanModule(const IKernel& oKernel) :
  IModule(oKernel),
  m_oDriver(oKernel.pInterface)
{
}

WindowsWlanModule::~WindowsWlanModule()
{
}

CcStatus WindowsWlanModule::init()
{
  m_oDriver.entry();
  return true;
}

CcStatus WindowsWlanModule::deinit()
{
  m_oDriver.unload();
  return true;
}
