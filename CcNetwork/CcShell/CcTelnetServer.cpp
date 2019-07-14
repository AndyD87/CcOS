/**
 * @copyright  Andreas Dirmeier (C) 2017
 *
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
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 * @brief     Implementation of Class CcTelnetServer
 */
#include "CcTelnetServer.h"
#include "CcKernel.h"
#include "CcTelnetServerWorker.h"

CcTelnetServer::CcTelnetServer(uint16 Port) :
m_Port(Port)
{
}

CcTelnetServer::~CcTelnetServer()
{
}

void CcTelnetServer::run()
{
  CCDEBUG("Telnet-Server starting on Port: " + CcString::fromNumber(m_Port));
  ISocket *temp;
  m_Socket = CcSocket(ESocketType::TCP);
  if (m_Socket.bind(m_Port))
  {
    if (m_Socket.listen())
    {
      while (getThreadState() == EThreadState::Running)
      {
        temp = m_Socket.accept();
        if (temp != nullptr)
        {
          CCNEWTYPE(pWorker,CcTelnetServerWorker,temp);
          pWorker->start();
        }
        else
        {
          CCDEBUG("CcTelnetServerServer::run Accept failed, wait a second for retry");
          CcKernel::delayS(1);
        }
      }
    }
    else
    {
      CCDEBUG("CcTelnetServerServer::run Listen failed, wait a second for retry");
      CcKernel::delayS(1);
    }
  }
  else
  {
    CCDEBUG("CcTelnetServerServer::run Bind failed, wait a second for retry");
    CcKernel::delayS(1);
  }
}
