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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcTftpServer
 *            Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */
#include "CcKernel.h"
#include "CcTftpServer.h"
#include "ETftpServer.h"

CcApp* CcTftpServer::main(CcStringList *Arg)
{
  CcApp* ret = new CcTftpServer(Arg); CCMONITORNEW(ret);
  return ret;
}

CcTftpServer::CcTftpServer( uint16 Port ) :
  m_Port(Port)
{
  m_sRootDir = CcKernel::getFileSystemManager().getWorkingDir();
}

CcTftpServer::CcTftpServer(CcStringList *Arg) :
  m_Port(27521)
{
  CCUNUSED(Arg);
}

CcTftpServer::~CcTftpServer( void )
{
  if (m_Socket != nullptr)
    m_Socket->close();
}

void CcTftpServer::run(void)
{
  CCDEBUG("TFTP-Server starting on Port: " + CcString::fromNumber(m_Port));
  m_Socket = CcKernel::getSocket(ESocketType::UDP);
  if (m_Socket->bind(m_Port))
  {
    while (getThreadState() == EThreadState::Running)
    {
      CcByteArray *oReceived = new CcByteArray(1024); // @todo: Magic number
      CCMONITORNEW(oReceived);
      m_Socket->readArray(*oReceived);
      CcSocket *oNewSocket = CcKernel::getSocket(ESocketType::UDP);
      oNewSocket->setPeerInfo(m_Socket->getPeerInfo());
      if (oNewSocket->bind(40980))
      {
        CcTftpServerWorker *worker = new CcTftpServerWorker(oReceived, oNewSocket, this); CCMONITORNEW(worker);
        worker->start();
      }
      else
        CCDEBUG("CcTftpServer::run Bind failed, wait a second for retry");
    }
  }
  else
  {
    CCDEBUG("CcTftpServer::run Bind failed, wait a second for retry");
    CcKernel::delayS(1);
  }
}