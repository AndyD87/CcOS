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
 * @brief     Implemtation of class CcDhcpServer
 */

#include "CcDhcpServer.h"
#include "CcString.h"
#include "Server/CWorker.h"
#include "Server/CConfig.h"
#include "Server/CData.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "Network/Protocols/Dhcp/CcDhcpPacket.h"

class CcDhcpServer::CPrivate
{
public:
  NDhcp::NServer::CConfig     oConfig;
  NDhcp::NServer::CData       oData;
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
      CCNEWTYPE(oPacket, CcDhcpPacket);
      size_t uiReadSize = oSocket.read(oPacket->getPacket(), oPacket->getPacketSize());
      if (uiReadSize != SIZE_MAX)
      {
        CCNEWTYPE(pWorker, NDhcp::NServer::CWorker, getConfig(), m_pPrivate->oData, oPacket);
        pWorker->start();
      }
      else
      {
        CCDELETE(oPacket);
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
  //return m_pPrivate->oConfig.loadConfigFile(sPath);
  return false;
}

void CcDhcpServer::initPrivate()
{
  CCNEW(m_pPrivate, CPrivate);
}

const NDhcp::NServer::CConfig& CcDhcpServer::getConfig()
{
  return m_pPrivate->oConfig;
}
