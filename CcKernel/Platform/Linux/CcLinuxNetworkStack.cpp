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
 * @brief     Implementation of class CcLinuxNetworkStack
 */
#include "CcLinuxNetworkStack.h"
#include "Network/ISocket.h"
#include "CcVector.h"
#include "CcStatic.h"
#include "CcLinuxSocketUdp.h"
#include "CcLinuxSocketTcp.h"

#include "CcDirectory.h"
#include "CcFileInfoList.h"

#include <ifaddrs.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */

bool CcLinuxNetworkStack::init()
{
  struct ifaddrs *pAdapters, *pAdapter;
  if(0 == getifaddrs(&pAdapters))
  {
    // loop through adapters
    for (pAdapter = pAdapters; pAdapter != nullptr; pAdapter = pAdapter->ifa_next)
    {
      if (pAdapter->ifa_addr && pAdapter->ifa_addr->sa_family==AF_PACKET)
      {
        struct sockaddr_ll* pMac = (struct sockaddr_ll*) pAdapter->ifa_addr;
        // loop addressses of adapter
        CcString sName(pAdapter->ifa_name);
        m_oDeviceList.append(new CcLinuxNetworkDevice(sName, pMac->sll_addr));
      }
    }
    freeifaddrs(pAdapters);
  }
  return true;
}

void CcLinuxNetworkStack::deinit()
{
  while(m_oDeviceList.size() > 0)
  {
    delete m_oDeviceList.at(0);
    m_oDeviceList.remove(0);
  }
}

const CcIpInterface* CcLinuxNetworkStack::getInterfaceForIp(const CcIp& oIp) const
{
  const CcIpInterface* pInterface = nullptr;
  for(const CcLinuxNetworkDevice* oDevice : m_oDeviceList)
  {
    for(const CcIpInterface& oInterface : oDevice->getInterfaceList())
    {
      if(oInterface.oIpAddress == oIp)
        return &oInterface;
    }
  }
  return pInterface;
}

ISocket* CcLinuxNetworkStack::getSocket(ESocketType eType)
{
  ISocket* pSocket = nullptr;
  switch (eType)
  {
    case ESocketType::TCP:
      CCNEW(pSocket, CcLinuxSocketTcp);
      break;
    case ESocketType::UDP:
      CCNEW(pSocket, CcLinuxSocketUdp);
      break;
    default:
      // Do nothing
      break;
  }
  return pSocket;
}


CcLinuxNetworkDevice* CcLinuxNetworkStack::getAdapterByName(const CcString& sName)
{
  for(CcLinuxNetworkDevice* oDevice : m_oDeviceList)
  {
    if(oDevice->getName() == sName)
      return oDevice;
  }
  return nullptr;
}
