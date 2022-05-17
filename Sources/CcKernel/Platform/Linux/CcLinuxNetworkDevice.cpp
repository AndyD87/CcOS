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
 * @brief     Implementation of class CcLinuxNetworkDevice
 */
#include "CcLinuxNetworkDevice.h"

#include <ifaddrs.h>
#include <arpa/inet.h>
#include <linux/if.h>

const char* CcLinuxNetworkDevice::c_sBasePath("/sys/class/net");

CcLinuxNetworkDevice::CcLinuxNetworkDevice(const CcString& sName, unsigned char pMacAddress[]):
  m_sName(sName),
  m_oAddress(pMacAddress, true)
{
  refreshInterfaces();
}

bool CcLinuxNetworkDevice::isConnected()
{
  return false;
}

void CcLinuxNetworkDevice::refreshInterfaces()
{
  struct ifaddrs *pAdapters, *pAdapter;

  if(0 == getifaddrs(&pAdapters))
  {
    // loop through adapters
    for (pAdapter = pAdapters; pAdapter != nullptr; pAdapter = pAdapter->ifa_next)
    {
      // loop addressses of adapter
      if (pAdapter->ifa_name == m_sName &&
          pAdapter->ifa_addr)
      {
        if(pAdapter->ifa_addr->sa_family == AF_INET)
        {
          CcIpInterface* pInterface;
          if((pInterface = m_oInterfaces.getInterfaceByIp(CcIp(((struct sockaddr_in *)pAdapter->ifa_addr)->sin_addr.s_addr, true))) == nullptr)
          {
            CcIpInterface oInterface;
            oInterface.oIpAddress.setIp(((struct sockaddr_in *)pAdapter->ifa_addr)->sin_addr.s_addr, true);
            oInterface.setSubnet(CcIp(((struct sockaddr_in *)pAdapter->ifa_netmask)->sin_addr.s_addr, true));
            oInterface.pDevice = this;
            m_oInterfaces.append(oInterface);
          }
          else
          {
            pInterface->oIpAddress.setIp(((struct sockaddr_in *)pAdapter->ifa_addr)->sin_addr.s_addr, true);
          }
        }
        else if(pAdapter->ifa_addr->sa_family == AF_PACKET )
        {

        }
      }
    }
    freeifaddrs(pAdapters);
  }
}
