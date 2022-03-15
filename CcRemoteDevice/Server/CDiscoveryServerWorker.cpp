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
#include "Server/CDiscoveryServerWorker.h"
#include "Network/CcSocket.h"
#include "CcKernel.h"
#include "CcString.h"
#include "Network/CcCommonPorts.h"
#include "Packets/CRequestDiscover.h"

namespace NRemoteDevice
{
namespace Server
{

void CDiscoveryServerWorker::run()
{
  CRequestDiscover* pPacket = m_oData.cast<CRequestDiscover>();

  if (pPacket && pPacket->getCommand() == ECommand::Discover)
  {
    CcSocket oSocket(ESocketType::UDP);
    oSocket.setPeerInfo(m_oPeerInfo);
    if (oSocket.open())
    {
      if (oSocket.writeArray(m_oData))
      {
        CCDEBUG("Query received");
      }
      else
      {
        CCERROR("Query response failed");
      }
    }
  }
}

}
}