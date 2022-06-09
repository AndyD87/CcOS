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
 * @copyright Andreas Dirmeier (C) 2020
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcFsDriver
 */
#include "CcFsDriver.h"
#include "CcFsDevice.h"

CcFsDriver::CcFsDriver(CcKernelModuleContext* pContext) :
  NKernelModule::IFsDriver(pContext)
{
}

CcFsDriver::~CcFsDriver()
{
}

bool CcFsDriver::checkDevice(NKernelModule::IDeviceInterface& CCUNUSED_PARAM(oTargetDevice))
{
  return false;
}

NKernelModule::IFsDevice* CcFsDriver::createDevice(NKernelModule::IDeviceInterface& oTargetDevice)
{
  CCNEWTYPE(pDevice, CcFsDevice, this, oTargetDevice);
  if (pDevice->start())
  {
    CCDEBUG("Device created");
  }
  else
  {
    CCDELETE(pDevice);
  }
  return pDevice;
}