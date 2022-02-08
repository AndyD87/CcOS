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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcRemoteDeviceDiscovery
 */
#include "Server/CInterfaceServer.h"
#include "Server/CInterfaceServerThread.h"
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

CInterfaceServer::~CInterfaceServer()
{
  m_oSocket.close();
}

void CInterfaceServer::run()
{
  do
  {
    if (isRunning() &&
        m_oServer.getConfig().bDetectable)
    {
      m_oSocket = CcSocket(ESocketType::TCP);
      if (!m_oSocket.open())
      {
        CCDEBUG("CInterfaceServer::run open failed");
      }
      else
      {
        m_oSocket.close();
      }
    }
  } while (isRunning() && CcKernel::sleep(1000));
}

void CInterfaceServer::onStop()
{
  m_oSocket.close();
}

}
}