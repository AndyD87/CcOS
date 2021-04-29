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
 * @brief     Implementation of class CcWindowsNetworkStack
 */

#include "CcWindowsGlobals.h"
#include "CcWindowsNetworkStack.h"
#include "Network/ISocket.h"
#include "CcVector.h"
#include "CcStatic.h"
#include "CcWindowsSocketUdp.h"
#include "CcWindowsSocketTcp.h"

#include "CcWmiInterface.h"

bool CcWindowsNetworkStack::init()
{
  // Get all devices
  CcString sQuery = "SELECT DeviceID FROM Win32_NetworkAdapter WHERE MACAddress IS NOT NULL";
  CcWmiInterface oInterface;
  if (oInterface.open())
  {
    CcWmiResult oResult =  oInterface.query(sQuery);
    for (CcTableRow& oRow : oResult)
    {
      uint32 uiId = oRow["DeviceID"].getUint32();
      if (uiId)
      {
        m_oDevices.append(new CcWindowsNetworkDevice(uiId));
      }
    }
    oInterface.close();
  }

  return true;
}

void CcWindowsNetworkStack::deinit()
{
  while (m_oDevices.size() > 0)
  {
    delete m_oDevices[0];
    m_oDevices.remove(0);
  }
}

const CcIpInterface* CcWindowsNetworkStack::getInterfaceForIp(const CcIp& oIp) const
{
  CcIpInterface* pIpSettings = nullptr;
  CCUNUSED(oIp);
  return pIpSettings;
}

ISocket* CcWindowsNetworkStack::getSocket(ESocketType eType)
{
  ISocket* pSocket = nullptr;
  switch (eType)
  {
    case ESocketType::TCP:
      CCNEW(pSocket, CcWindowsSocketTcp);
      break;
    case ESocketType::UDP:
      CCNEW(pSocket, CcWindowsSocketUdp);
      break;
    default:
      // Do nothing
      break;
  }
  return pSocket;
}
