/*
 * This file is part of CcRemoteDeviceDiscovery.
 *
 * CcRemoteDeviceDiscovery is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceDiscovery is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceDiscovery.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcRemoteDeviceDiscovery
 */
#include "Server/CDiscoveryServer.h"
#include "Server/CDiscoveryServerWorker.h"
#include "Server/CConfig.h"
#include "Network/CcSocket.h"
#include "CcKernel.h"
#include "CcRemoteDeviceServer.h"
#include "CcString.h"
#include "Network/CcCommonPorts.h"
#include "Packets/CRequestDiscover.h"

namespace NRemoteDevice
{
namespace Server
{

CDiscoveryServer::~CDiscoveryServer()
{
  m_oSocket.close();
}

void CDiscoveryServer::run()
{
  do
  {
    m_oSocket = CcSocket(ESocketType::UDP);
    if (!m_oSocket.open())
    {
      CCDEBUG("CcRemoteDeviceServer::run open failed");
    }
    else
    {
      if (!m_oSocket.setOption(ESocketOption::Reuse))
      {
        CCDEBUG("CcRemoteDeviceServer::run reuse failed");
      }
      else if (!m_oSocket.setOption(ESocketOption::Broadcast))
      {
        CCDEBUG("CcRemoteDeviceServer::run broadcast failed");
      }
      else if (m_oSocket.bind(m_oServer.getConfig().oAddressInfo))
      {
        while (isRunning())
        {
          CCNEWTYPE(pWorker, CDiscoveryServerWorker, m_oServer);
          size_t uiReadSize = m_oSocket.readArray(pWorker->getData());
          if (uiReadSize != SIZE_MAX &&
              uiReadSize > 0)
          {
            pWorker->getPeerInfo() = m_oSocket.getPeerInfo();
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
        CCDEBUG("CcRemoteDeviceServer::run Bind failed");
      }
      m_oSocket.close();
    }
  } while (isRunning() &&
           m_oServer.getConfig().bDetectable &&
           CcKernel::sleep(100));
}

void CDiscoveryServer::onStop()
{
  m_oSocket.close();
}

}
}