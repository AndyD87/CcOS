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
#include "Network/CcCommonIps.h"

class CcDhcpServer::CPrivate
{
public:
  CPrivate() :
    oSocket(ESocketType::UDP)
  {}
  NDhcp::NServer::CConfig     oConfig;
  NDhcp::NServer::CData       oData;
  CcSocket                    oSocket;
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
  if (m_pPrivate->oSocket.open() &&
      m_pPrivate->oSocket.setOption(ESocketOption::Reuse) &&
      m_pPrivate->oSocket.setOption(ESocketOption::Broadcast) &&
      m_pPrivate->oSocket.bind(getConfig().getBindAddress()))
  {
    while (getThreadState() == EThreadState::Running)
    {
      CCNEWTYPE(pWorker, NDhcp::NServer::CWorker, *this, m_pPrivate->oData);
      size_t uiReadSize = m_pPrivate->oSocket.read(pWorker->getPacket()->getPacket(), pWorker->getPacket()->getPacketSize(true));
      if (uiReadSize != SIZE_MAX)
      {
        pWorker->start();
      }
      else
      {
        CCDELETE(pWorker);
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

size_t CcDhcpServer::write(void* pBuf, size_t uiBufSize)
{
  CcSocketAddressInfo oPeerInfo(ESocketType::UDP, CcCommonIps::Broadcast, CcCommonPorts::DHCP_CLI);
  m_pPrivate->oSocket.setPeerInfo(oPeerInfo);
  return m_pPrivate->oSocket.write(pBuf, uiBufSize);
}

void CcDhcpServer::initPrivate()
{
  CCNEW(m_pPrivate, CPrivate);
}

const NDhcp::NServer::CConfig& CcDhcpServer::getConfig()
{
  return m_pPrivate->oConfig;
}
