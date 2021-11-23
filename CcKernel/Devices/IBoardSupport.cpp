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
 * @brief     Class IBoardSupport
 */
#include "Devices/IBoardSupport.h"

IBoardSupport::~IBoardSupport()
{
  for(CHwDevice& pHwDevice : m_oHwDevices)
  {
    CCDELETE(pHwDevice.pDevice);
  }
  m_oHwDevices.clear();
}

uint32 IBoardSupport::getDeviceSize(EDeviceType eDeviceType) const
{
  uint32 uiSize = 0;
  for(IBoardSupport::CHwDevice& oDevice : m_oHwDevices)
  {
    if(oDevice.eDevice == eDeviceType)
    {
      uiSize++;
    }
  }
  return uiSize;
}

IBoardSupport::CHwDevice& IBoardSupport::getHwDevice(EDeviceType eDeviceType, uint32 uiDeviceNumber)
{
  for(IBoardSupport::CHwDevice& oDevice : m_oHwDevices)
  {
    if(oDevice.eDevice == eDeviceType && oDevice.uiDeviceNr == uiDeviceNumber)
    {
      return oDevice;
    }
  }
  return InvalidDevice;
}
