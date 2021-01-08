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
 * @brief     Implementation of class CcRawSocketNetwork
 **/

#include "CcRawSocketNetwork.h"
#include "IThread.h"
#include "CcKernel.h"
#include <arpa/inet.h>
#include <linux/if_link.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netdb.h>

class CcRawSocketNetwork::CPrivate : public IThread
{
public:
  CPrivate(CcRawSocketNetwork* pParent) :
    pParent(pParent)
    {}
  ~CPrivate()
    {}

  virtual void run() override
  {
    while(getThreadState() == EThreadState::Running)
      pParent->readFrame();
  }

  CcRawSocketNetwork* pParent = nullptr;
  CcMacAddress    oMacAddress = CcMacAddress(0x00, 0x00, 0x12, 0x34, 0x56, 0x78);
};

CcRawSocketNetwork::CcRawSocketNetwork(unsigned long uiIndex)
{
  CCNEW(m_pPrivate, CPrivate, this);
  m_pPrivate->start();
  CCUNUSED(uiIndex);
}

CcRawSocketNetwork::CcRawSocketNetwork(const CcString& sName)
{
  CCNEW(m_pPrivate, CPrivate, this);
  m_pPrivate->start();
  CCUNUSED(sName);
}

CcRawSocketNetwork::~CcRawSocketNetwork()
{
  m_pPrivate->stop();
  CCDELETE(m_pPrivate);
}

const CcMacAddress& CcRawSocketNetwork::getMacAddress()
{
  return m_pPrivate->oMacAddress;
}

bool CcRawSocketNetwork::isConnected()
{
  bool bRet = false;
  return bRet;
}

uint32 CcRawSocketNetwork::getChecksumCapabilities()
{
  return 0;
}

void CcRawSocketNetwork::readFrame()
{
}

bool CcRawSocketNetwork::writeFrame(const CcNetworkPacketRef oFrame)
{
  CCUNUSED(oFrame);
  return SIZE_MAX;
}

unsigned long CcRawSocketNetwork::getAdapterCount()
{
  unsigned long uiCount = 0;
  struct ifaddrs *ifaddr, *ifa;
  int family, s, n;
  char host[NI_MAXHOST];

  if (getifaddrs(&ifaddr) == -1)
  {
      //perror("getifaddrs");
  }
  else
  {
    /* Walk through linked list, maintaining head pointer so we
       can free list later */
    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++)
    {
        if (ifa->ifa_addr == NULL)
            continue;
        family = ifa->ifa_addr->sa_family;
        /* Display interface name and family (including symbolic
           form of the latter for the common families) */
        //printf("%-8s %s (%d)\n",
        //       ifa->ifa_name,
        //       (family == AF_PACKET) ? "AF_PACKET" :
        //       (family == AF_INET) ? "AF_INET" :
        //       (family == AF_INET6) ? "AF_INET6" : "???",
        //       family);

        /* For an AF_INET* interface address, display the address */
        if (family == AF_INET || family == AF_INET6)
        {
            s = getnameinfo(ifa->ifa_addr,
                    (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                          sizeof(struct sockaddr_in6),
                    host, NI_MAXHOST,
                    NULL, 0, NI_NUMERICHOST);
            if (s == 0)
            {
              uiCount++;
                //printf("getnameinfo() failed: %s\n", gai_strerror(s));
            }
            //printf("\t\taddress: <%s>\n", host);
        }
        else if (family == AF_PACKET && ifa->ifa_data != NULL)
        {
            //struct rtnl_link_stats *stats = ifa->ifa_data;

            //printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
            //       "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
            //       stats->tx_packets, stats->rx_packets,
            //       stats->tx_bytes, stats->rx_bytes);
        }
    }
    freeifaddrs(ifaddr);
  }
  return uiCount;
}
