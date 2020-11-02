/*
 * This file is part of CcRemoteDeviceDiscovery.
 *
 * CcRemoteDeviceDiscovery is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceDiscovery is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceDiscovery.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcRemoteDeviceDiscovery
 */
#include "CcRemoteDeviceDiscovery.h"
#include "Network/CcSocket.h"
#include "CcKernel.h"
#include "CcDateTime.h"
#include "Network/CcCommonIps.h"
#include "Network/CcCommonPorts.h"
#include "CcRemoteDeviceGlobals.h"

void CcRemoteDeviceDiscovery::findAllDevices()
{
  CcSocketAddressInfo oSocketAddressInfo(ESocketType::UDP, CcCommonIps::Broadcast, CcCommonPorts::CcRemoteDevice);
  CcSocket oSocket = CcKernel::getSocket(ESocketType::UDP);
  oSocket.setOption(ESocketOption::Broadcast);
  oSocket.setOption(ESocketOption::Reuse);
  oSocket.setPeerInfo(oSocketAddressInfo);
  CcDateTime oStop = CcKernel::getUpTime();
  oStop.addSeconds(5);
  if (oSocket.open())
  {
    oSocket.write("Hallo", 60);
    while (CcKernel::getUpTime() < oStop)
    {
    }
  }
}
