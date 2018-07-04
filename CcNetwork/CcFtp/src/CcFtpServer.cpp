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
 * @brief     Implementation of Class CcFtpServer
 *            Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */
#include "CcKernel.h"
#include "CcFtpServer.h"
#include "CcFtpTypes.h"

CcApp* CcFtpServer::main(CcStringList *Arg)
{
  CcApp* ret = new CcFtpServer(Arg); CCMONITORNEW(ret);
  return ret;
}

CcFtpServer::CcFtpServer( uint16 Port ) :
  m_Port(Port)
{
}

CcFtpServer::CcFtpServer(CcStringList *Arg) :
  m_Port(CcCommonPorts::FTP)
{
  CCUNUSED(Arg);
}

CcFtpServer::~CcFtpServer( void )
{
  CCDELETE(m_pAnonymousUser);
}

void CcFtpServer::run(void)
{
  CCDEBUG("FTP-Server starting on Port: " + CcString::fromNumber(m_Port));
  m_Socket = CcSocket(ESocketType::TCP);
  if (m_Socket.bind(m_Port))
  {
    if (m_Socket.listen())
    {
      while (getThreadState() == EThreadState::Running)
      {
        CcSocketAbstract *temp;
        temp = m_Socket.accept();
        if (temp != nullptr)
        {
          CcFtpServerWorker *worker = new CcFtpServerWorker(temp, this); CCMONITORNEW(worker);
          worker->start();
        }
        else
        {
          CCDEBUG("CcFtpServer::run Accept failed, wait a second for retry");
          CcKernel::delayS(1);
        }
      }
    }
    else
    {
      CCDEBUG("CcFtpServer::run Listen failed, wait a second for retry");
      CcKernel::delayS(1);
    }
  }
  else
  {
    CCDEBUG("CcFtpServer::run Bind failed, wait a second for retry");
    CcKernel::delayS(1);
  }
}

void CcFtpServer::setUserList(CcUserList UserList)
{
  m_UserList = UserList;
}

void CcFtpServer::setAnonymous(bool bEnable)
{
  // find if user is existing
  CcUserHandle pUser = m_UserList.findUser("anonymous");
  // decide if delete or add user to list
  if (bEnable == true && m_pAnonymousUser == nullptr)
  {
    m_pAnonymousUser = new CcUser("anonymous");
    CCMONITORNEW(m_pAnonymousUser);
    pUser = m_pAnonymousUser;
    m_UserList.append(pUser);
  }
  else if (bEnable == false && pUser != nullptr)
  {
    m_UserList.removeItem(pUser);
  }
}

CcUserList &CcFtpServer::getUserList(void)
{
  return m_UserList;
}
