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


bool CcWindowsNetworkStack::init()
{
  // Get all devices
  // WMI Way for getting all devices
  CcString sQuery = "SELECT DeviceID FROM Win32_NetworkAdapter WHERE MACAddress IS NOT NULL";
  if (m_oInterface.open())
  {
    CcWmiResult oResult = m_oInterface.query(sQuery);
    for (CcTableRow& oRow : oResult)
    {
      uint32 uiId = oRow["DeviceID"].getUint32();
      if (uiId)
      {
        m_oDevices.append(CCNEW_INLINE(CcWindowsNetworkDevice, this, uiId));
      }
    }
  }

  return true;
}

void CcWindowsNetworkStack::deinit()
{
  while (m_oDevices.size() > 0)
  {
    CCDELETE(m_oDevices[0]);
    m_oDevices.remove(0);
  }
  // Force close to avoid using members at the end of live.
  m_oInterface.close(true);
}

const CcIpInterface* CcWindowsNetworkStack::getInterfaceForIp(const CcIp& oIp) const
{
  const CcIpInterface* pInterface = nullptr;
  for(const CcWindowsNetworkDevice* oDevice : m_oDevices)
  {
    for(const CcIpInterface& oInterface : oDevice->getInterfaceList())
    {
      if(oInterface.oIpAddress == oIp)
        return &oInterface;
    }
  }
  return pInterface;
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
