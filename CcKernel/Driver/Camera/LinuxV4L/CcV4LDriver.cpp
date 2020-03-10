/*
 * @copyright  Andreas Dirmeier (C) 2015
 *
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
 * @brief     Implementation of Class CcV4LDriver
 */
#include "CcV4LDriver.h"
#include "CcString.h"
#include "CcStatic.h"
#include "CcFile.h"
#include "CcV4LCamera.h"
#include "IKernel.h"

CcV4LDriver::CcV4LDriver(IKernel* pKernel) :
  m_pKernel(pKernel)
{
}

CcV4LDriver::~CcV4LDriver()
{
}

CcStatus CcV4LDriver::entry()
{
  CcStatus oStatus(false);
  if(CcFile::exists("/dev/video0"))
  {
    CCNEWTYPE(pCamera, CcV4LCamera);
    m_pKernel->addDevice(CcDeviceHandle(pCamera, EDeviceType::Camera));
  }
  return oStatus;
}

CcStatus CcV4LDriver::unload()
{
  return true;
}
