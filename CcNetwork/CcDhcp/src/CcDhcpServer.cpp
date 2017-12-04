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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcDhcpServer
 */

#include "CcDhcpServer.h"
#include "CcString.h"
#include "CcDhcpServerWorker.h"
#include "CcDhcpServerConfig.h"
#include "CcDhcpServerData.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcDhcpPacket.h"

class CcDhcpServerPrivate
{
public:
  CcDhcpServerConfig oConfig;
  CcDhcpServerData   oData;
};

CcDhcpServer::CcDhcpServer(void)
{
  initPrivate();
}

CcDhcpServer::~CcDhcpServer(void)
{
  CCDELETE(m_pPrivate);
}

void CcDhcpServer::run()
{
  CCDEBUG("DHCP-Server starting on Port: " + m_pPrivate->oConfig.getBindAddress().getPortString());
  CcSocket oSocket(ESocketType::UDP);
  if (oSocket.open() &&
      oSocket.setOption(ESocketOption::Reuse) &&
      oSocket.setOption(ESocketOption::Broadcast) &&
      oSocket.bind(m_pPrivate->oConfig.getBindAddress()))
  {
    while (getThreadState() == EThreadState::Running)
    {
      CcDhcpPacket oPacket;
      size_t uiReadSize = oSocket.read(oPacket.packet(), oPacket.packetSize());
      if (uiReadSize != SIZE_MAX)
      {
        CcSocketAddressInfo oLocalInfo = oSocket.getPeerInfo();
        CcDhcpServerWorker* pWorker = new CcDhcpServerWorker(m_pPrivate->oConfig, m_pPrivate->oData, std::move(oPacket));
        pWorker->start();
      }
    }
  }
  else
  {
    CCDEBUG("DHCP::run Bind failed");
  }
}

void CcDhcpServer::initPrivate()
{
  m_pPrivate = new CcDhcpServerPrivate();
  CCMONITORNEW(m_pPrivate);
}
