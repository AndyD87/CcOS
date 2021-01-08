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
 * @brief     Implementation of class CcTcpEchoServer
 */
#include "Network/Server/CcTcpEchoServer.h"
#include "CcStringList.h"
#include "Network/CcSocket.h"
#include "IWorker.h"
#include "CcByteArray.h"
#include "CcConsole.h"

class CcTcpEchoServerWorker : public IWorker
{
public:
  CcTcpEchoServerWorker(CcSocket oSocket) :
    IWorker("CcTcpEchoServerWorker"),
    pSocket(oSocket)
  {}

  ~CcTcpEchoServerWorker()
  {
    pSocket.close();
  }

  virtual void run() override
  {
    CcByteArray oData = pSocket.readAll();
    pSocket.writeArray(oData);
  }
private:
  CcSocket pSocket;
};

void CcTcpEchoServer::run()
{
  if (m_pSocket == nullptr)
  {
    CCNEW(m_pSocket, CcSocket, ESocketType::TCP);
  }
  else
  {
    m_bSocketCreated = true;
  }
  setExitCode(EStatus::Error);
  m_pSocket->setOption(ESocketOption::ReusePort);
  if (m_pSocket->bind(m_uiPort))
  {
    if (m_pSocket->listen())
    {
      ISocket *temp;
      while (getThreadState() == EThreadState::Running)
      {
        temp = m_pSocket->accept();
        if(temp != nullptr)
        {
          CCNEWTYPE(worker, CcTcpEchoServerWorker, CcSocket(temp));
          worker->start();
        }
      }
    }
    else
    {
      setExitCode(EStatus::NetworkPortInUse);
    }
  }
  else
  {
    setExitCode(EStatus::NetworkPortInUse);
  }
  // Check if nothing changed since init
  if (getExitCode() == EStatus::Error)
  {
    setExitCode(EStatus::AllOk);
  }
}
