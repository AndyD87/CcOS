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
 * @brief     Implementation of Class CcGGphotoCamera
 */

#include "CcWindowsDesktopScreenModule.h"
#include "CcWindowsDesktopScreenDevice.h"
#include "CcKernel.h"

CCEXTERNC CcWindowsDesktopScreenSHARED IModule* IModule_Create()
{
  CCNEWTYPE(pModule, CcWindowsDesktopScreenModule);
  return pModule;
}

CCEXTERNC CcWindowsDesktopScreenSHARED void IModule_Remove(IModule* pModule)
{
  CCDELETE(pModule);
}

CcWindowsDesktopScreenModule::CcWindowsDesktopScreenModule()
{
}

CcWindowsDesktopScreenModule::~CcWindowsDesktopScreenModule()
{
}

CcStatus CcWindowsDesktopScreenModule::init()
{
  CCNEW(m_pCamera, CcWindowsDesktopScreenDevice);
  CcKernel::addDevice(CcDeviceHandle(EDeviceType::Camera, m_pCamera));
  return true;
}

CcStatus CcWindowsDesktopScreenModule::deinit()
{
  CCDELETE(m_pCamera);
  return true;
}
