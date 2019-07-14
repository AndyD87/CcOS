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
#include "CcDhcpServerConfigFile.h"
#include "CcDhcpServerData.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "Network/Protocols/Dhcp/CcDhcpPacket.h"

class CcDhcpServer::CPrivate
{
public:
  CcDhcpServerConfigFile oConfigFile;
  CcDhcpServerData   oData;
};

CcDhcpServer::CcDhcpServer()
{
  initPrivate();
}

CcDhcpServer::~CcDhcpServer()
{
  CCDELETE(m_pPrivate);
}

void CcDhcpServer::run()
{
  CCDEBUG("DHCP-Server starting on Port: " + getConfig().getBindAddress().getPortString());
  CcSocket oSocket(ESocketType::UDP);
  if (oSocket.open() &&
      oSocket.setOption(ESocketOption::Reuse) &&
      oSocket.setOption(ESocketOption::Broadcast) &&
      oSocket.bind(getConfig().getBindAddress()))
  {
    while (getThreadState() == EThreadState::Running)
    {
      CcDhcpPacket oPacket;
      size_t uiReadSize = oSocket.read(oPacket.packet(), oPacket.packetSize());
      if (uiReadSize != SIZE_MAX)
      {
        CcSocketAddressInfo oLocalInfo = oSocket.getPeerInfo();
        CcDhcpServerWorker* pWorker = new CcDhcpServerWorker(getConfig(), m_pPrivate->oData, std::move(oPacket));
        pWorker->start();
      }
    }
  }
  else
  {
    CCDEBUG("DHCP::run Bind failed");
  }
}

bool CcDhcpServer::loadConfigFile(const CcString& sPath)
{
  return m_pPrivate->oConfigFile.loadConfigFile(sPath);
}

void CcDhcpServer::initPrivate()
{
  CCNEW(m_pPrivate, CPrivate);
}

const CcDhcpServerConfig& CcDhcpServer::getConfig()
{
  return m_pPrivate->oConfigFile.getConfig();
}
