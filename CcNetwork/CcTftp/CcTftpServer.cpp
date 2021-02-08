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
 * @brief     Implementation of Class CcTftpServer
 *            Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */
#include "CcKernel.h"
#include "CcTftpServer.h"
#include "CcTftpGlobals.h"
#include "ETftpServer.h"

//! Temporary file transfer size for tftp
static uint16 g_uiTemp = 40980;

CcTftpServer::CcTftpServer()
{
}

CcTftpServer::CcTftpServer(const CcTftpServerConfig& oConfig) :
  m_oConfig(oConfig)
{
}


CcTftpServer::~CcTftpServer()
{
}

void CcTftpServer::run()
{
  CCDEBUG("TFTP-Server starting on Port: " + CcString::fromNumber(m_oConfig.getPort()));
  m_Socket = CcSocket(ESocketType::UDP);
  if (m_Socket.bind(m_oConfig.getPort()))
  {
    while (getThreadState() == EThreadState::Running)
    {
      CCNEWTYPE(oReceived, CcByteArray, 1024); // @todo: Magic number
      m_Socket.readArray(*oReceived);
      CcSocket oNewSocket(ESocketType::UDP);
      oNewSocket.setPeerInfo(m_Socket.getPeerInfo());
      if (oNewSocket.bind(g_uiTemp++))
      {
        CCDEBUG("CcTftpServer: incomming connection.");
        CCNEWTYPE(worker, CcTftpServerWorker, oReceived, oNewSocket, &m_oConfig);
        worker->start();
      }
      else
      {
        CCDEBUG("CcTftpServer::run Client Bind failed");
      }
    }
  }
  else
  {
    CCDEBUG("CcTftpServer::run Bind failed");
  }
}
