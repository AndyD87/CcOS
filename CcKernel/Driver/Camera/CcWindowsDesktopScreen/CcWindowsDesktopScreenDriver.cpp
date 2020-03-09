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
 * @brief     Implementation of Class CcWindowsDesktopScreenDriver
 *
 * @look https://technet.microsoft.com/de-de/library/security/dd145119
 */
#include "Driver/Camera/CcWindowsDesktopScreen/CcWindowsDesktopScreen.h"
#include "Driver/Camera/CcWindowsDesktopScreen/CcWindowsDesktopScreenDriver.h"
#include "Driver/Camera/CcWindowsDesktopScreen/CcWindowsDesktopScreenDevice.h"
#include "CcKernel.h"
#include "IDevice.h"

CcWindowsDesktopScreenDriver::CcWindowsDesktopScreenDriver()
{
  CCNEW(m_DesktopScreen, CcWindowsDesktopScreenDevice);
}

CcWindowsDesktopScreenDriver::~CcWindowsDesktopScreenDriver()
{
  CCDELETE(m_DesktopScreen);
}

CcStatus CcWindowsDesktopScreenDriver::entry()
{
  CcStatus oStatus;
  CcKernel::addDevice(CcDeviceHandle(m_DesktopScreen, EDeviceType::Camera));
  return oStatus;
}

CcStatus CcWindowsDesktopScreenDriver::unload()
{
  CcStatus oStatus;
  CCDELETE(m_DesktopScreen);
  return oStatus;
}
