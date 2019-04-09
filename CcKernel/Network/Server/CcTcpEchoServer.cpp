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
    pSocket(oSocket)
  {}

  ~CcTcpEchoServerWorker()
  {
    pSocket.close();
  }

  virtual void run() override
  {
    pSocket.writeArray("");
  }
private:
  CcSocket pSocket;
};

void CcTcpEchoServer::run()
{
  setExitCode(EStatus::Error);
  CcSocket oSocket(ESocketType::TCP);
  oSocket.setOption(ESocketOption::ReusePort);
  if (oSocket.bind(m_uiPort))
  {
    if (oSocket.listen())
    {
      ISocket *temp;
      while (getThreadState() == EThreadState::Running)
      {
        temp = oSocket.accept();
        CcTcpEchoServerWorker *worker = new CcTcpEchoServerWorker(CcSocket(temp)); CCMONITORNEW(worker);
        worker->start();
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
