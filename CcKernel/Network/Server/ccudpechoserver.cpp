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
 * @brief     Implementation of class CcUdpEchoServer
 */
#include "Network/Server/CcUdpEchoServer.h"
#include "CcStringList.h"
#include "Network/CcSocket.h"
#include "IWorker.h"
#include "CcByteArray.h"
#include "CcConsole.h"

class CcUdpEchoServerWorker : public IWorker
{
public:
  CcUdpEchoServerWorker(CcSocket oSocket, CcByteArray* pInData) :
    m_pSocket(oSocket),
    m_pInData(pInData)
  {}

  ~CcUdpEchoServerWorker()
  {
    m_pSocket.close();
    delete m_pInData;
  }

  virtual void run() override
  {
    m_pSocket.writeArray(*m_pInData);
  }
private:
  CcSocket m_pSocket;
  CcByteArray* m_pInData;
};

void CcUdpEchoServer::run()
{
  CcSocket oSocket(ESocketType::UDP);
  if (oSocket.bind(m_uiPort))
  {
    while (getThreadState() == EThreadState::Running)
    {
      CcByteArray* pInData = new CcByteArray(1520);
      CCMONITORNEW(pInData);
      oSocket.readArray(*pInData);
      CcConsole::writeArray(*pInData);
      if (m_uiResponse)
      {
        CcSocket oNewSocket(ESocketType::UDP);
        CcSocketAddressInfo oAddress = oSocket.getPeerInfo();
        oAddress.setPort(m_uiResponse);
        if (oNewSocket.open())
        {
          oNewSocket.setPeerInfo(oAddress);
          CcUdpEchoServerWorker *worker = new CcUdpEchoServerWorker(oNewSocket, pInData);
          CCMONITORNEW(worker);
          worker->start();
        }
        else
        {
          CCDEBUG("CcTftpServer::run Client Bind failed");
        }
      }
      else
      {
        delete pInData;
      }
    }
  }
}
