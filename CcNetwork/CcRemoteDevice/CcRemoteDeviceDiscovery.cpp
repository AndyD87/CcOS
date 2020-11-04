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
#include "CcRemoteDeviceGlobals.h"
#include "CcByteArray.h"
#include "Packets/CRequestDiscover.h"

size_t CcRemoteDeviceDiscovery::findAllDevices(uint16 uiPort, const CcDateTime& oWaitTime)
{
  clear();
  CcSocketAddressInfo oSocketAddressInfo(ESocketType::UDP, CcCommonIps::Broadcast, uiPort);
  m_oSocket.setPeerInfo(oSocketAddressInfo);
  CcDateTime oStop = CcKernel::getUpTime();
  oStop += oWaitTime;
  if (m_oSocket.open())
  {
    m_oSocket.setOption(ESocketOption::Broadcast);
    if (bind())
    {
      NRemoteDevice::CRequestDiscover oPaket;
      m_oSocket.setTimeout(CcDateTimeFromSeconds(1));
      m_oSocket.write(&oPaket, sizeof(oPaket));
      CcByteArray oData(1024);
      while (CcKernel::getUpTime() < oStop)
      {
        size_t uiRead = m_oSocket.readArray(oData);
        if (uiRead && uiRead <= oData.size())
        {
          append(m_oSocket.getPeerInfo());
        }
      }
    }
  }
  return size();
}

bool CcRemoteDeviceDiscovery::bind()
{
  CcStatus oSuccess = false;
  uint16 uiPortBase = CcCommonPorts::CcRemoteDevice + CcCommonPorts::CcSourceBase;
  while ( uiPortBase < CcCommonPorts::CcRemoteDevice + CcCommonPorts::CcSourceBase + 500 &&
          oSuccess == false)
  {
    CcSocketAddressInfo oSocketAddressInfo(ESocketType::UDP, CcCommonIps::AnyAddress, uiPortBase);
    m_oSocket.setAddressInfo(oSocketAddressInfo);
    oSuccess = m_oSocket.bind(uiPortBase);
    uiPortBase++;
  }
  return oSuccess;
}